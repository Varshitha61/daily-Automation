"""
main.py — Central orchestrator for the Daily Coding Challenge Bot.

Runs the full daily pipeline:
  1. Validates configuration
  2. Initialises the SQLite database
  3. For each platform (LeetCode, Codeforces, CodeChef, HackerRank):
     a. Skips if already solved today
     b. Fetches the daily problem
     c. Solves it via Claude AI
     d. Saves the solution to disk
     e. Records the result in the database
     f. Sends a Telegram success notification
     g. Sends a Telegram error notification on any failure
  4. Sends a daily summary notification

Entry points:
    python main.py               — run once immediately
    python scheduler.py          — run on the configured daily schedule
"""

import asyncio
import logging
import os
import re
import sys
from datetime import date
from pathlib import Path
from typing import Optional

from config import Config
from storage import db
from notifier import telegram
from cookie_checker import send_cookie_warnings

# Platform fetchers — imported lazily inside the pipeline to prevent a single
# bad import from breaking the entire run.
import platforms.leetcode as lc_fetcher
import platforms.codeforces as cf_fetcher
import platforms.codechef as cc_fetcher
import platforms.hackerrank as hr_fetcher

from solver.groq_solver import solve_problem

import platforms.submitters.leetcode_submit as lc_submit
import platforms.submitters.codeforces_submit as cf_submit
import platforms.submitters.codechef_submit as cc_submit
import platforms.submitters.hackerrank_submit as hr_submit

_SUBMITTERS = {
    "leetcode": lc_submit.submit_solution,
    "leetcode_2": lc_submit.submit_solution,
    "codeforces": cf_submit.submit_solution,
    "codechef": cc_submit.submit_solution,
    "hackerrank": hr_submit.submit_solution,
}


# ---------------------------------------------------------------------------
# Logging setup
# ---------------------------------------------------------------------------

_LOG_FILE: Path = Path(__file__).parent / "logs" / "bot.log"


def _configure_logging() -> None:
    """
    Configure the root Python logger to write to both the console (stdout)
    and a rotating log file at logs/bot.log.

    The log level is read from Config.LOG_LEVEL.  Both handlers share the
    same formatter so output is consistent.
    """
    _LOG_FILE.parent.mkdir(parents=True, exist_ok=True)

    log_format = "%(asctime)s [%(levelname)-8s] %(name)s — %(message)s"
    date_format = "%Y-%m-%d %H:%M:%S"

    root_logger = logging.getLogger()
    root_logger.setLevel(Config.log_level_int())

    # Console handler — force UTF-8 so Windows cp1252 doesn't choke on
    # Unicode box-drawing characters in log messages.
    console_handler = logging.StreamHandler(
        open(sys.stdout.fileno(), mode="w", encoding="utf-8", closefd=False)
    )
    console_handler.setLevel(Config.log_level_int())
    console_handler.setFormatter(logging.Formatter(log_format, datefmt=date_format))

    # File handler
    file_handler = logging.FileHandler(_LOG_FILE, encoding="utf-8")
    file_handler.setLevel(logging.DEBUG)  # always log everything to file
    file_handler.setFormatter(logging.Formatter(log_format, datefmt=date_format))

    root_logger.addHandler(console_handler)
    root_logger.addHandler(file_handler)


logger = logging.getLogger(__name__)

# ---------------------------------------------------------------------------
# Solution saver (Phase 6)
# ---------------------------------------------------------------------------

_SOLUTIONS_BASE: Path = Path(__file__).parent / "solutions"


def _sanitise_filename(text: str) -> str:
    """
    Convert a problem title into a safe, lowercase filesystem name.

    Replaces spaces with underscores, strips characters that are not
    alphanumeric, underscores, or hyphens, and collapses consecutive
    underscores.

    Args:
        text (str): Raw problem title, e.g. "Two Sum".

    Returns:
        str: Sanitised filename component, e.g. "two_sum".
    """
    text = text.lower().strip()
    text = re.sub(r"[^\w\s-]", "", text)       # remove special chars
    text = re.sub(r"[\s]+", "_", text)          # spaces → underscores
    text = re.sub(r"_+", "_", text)             # collapse multiple underscores
    return text.strip("_")


def save_solution(solved: dict) -> str:
    """
    Write the AI-generated solution code to a dated file inside the
    appropriate platform sub-directory, then record the save in the database.

    File path format:
        solutions/{platform}/YYYY-MM-DD_{sanitised_title}.py

    Example:
        solutions/leetcode/2024-01-15_two_sum.py

    Args:
        solved (dict): The solution dict returned by solve_problem().
                       Required keys: code, language, problem_title, platform.
                       Optional (passed through to db.log_solution):
                           url — problem URL string.

    Returns:
        str: The absolute file path of the saved solution file.

    Raises:
        OSError: If the directory cannot be created or the file cannot be
                 written (e.g. permission denied).
    """
    platform: str = solved["platform"]
    title: str = solved["problem_title"]
    code: str = solved["code"]
    language: str = solved["language"]
    url: Optional[str] = solved.get("url")

    today_str: str = date.today().isoformat()
    safe_title: str = _sanitise_filename(title)

    # Derive the file extension from the language
    ext_map: dict[str, str] = {
        "python3": "py",
        "python": "py",
        "cpp": "cpp",
        "c++": "cpp",
        "java": "java",
        "javascript": "js",
        "typescript": "ts",
        "go": "go",
        "rust": "rs",
    }
    ext: str = ext_map.get(language.lower(), "txt")

    platform_dir: Path = _SOLUTIONS_BASE / platform
    platform_dir.mkdir(parents=True, exist_ok=True)

    file_path: Path = platform_dir / f"{today_str}_{safe_title}.{ext}"

    logger.info("Saving solution to %s", file_path)
    file_path.write_text(code, encoding="utf-8")

    # Record in the database
    db.log_solution(
        platform=platform,
        title=title,
        url=url,
        language=language,
        file_path=str(file_path),
        status=solved.get("status", "saved"),
        submission_id=solved.get("submission_id"),
    )

    logger.info("Solution saved and logged — %s", file_path.name)
    return str(file_path)


# ---------------------------------------------------------------------------
# Per-platform pipeline step
# ---------------------------------------------------------------------------

async def _run_platform(platform_name: str, fetch_fn) -> bool:
    """
    Execute the full fetch → solve → save → notify pipeline for one platform.

    Designed to be called from run_daily_bot() inside an asyncio event loop.
    Uses asyncio.to_thread() to run blocking I/O (HTTP, Playwright) off the
    main thread without blocking the event loop.

    Args:
        platform_name (str): Human-readable platform identifier used in log
                             messages and Telegram notifications.
        fetch_fn      (callable): The zero-argument fetch function for this
                                  platform (e.g. lc_fetcher.fetch_daily_problem).

    Returns:
        bool: True if the platform completed successfully; False if any step
              raised an exception.
    """
    logger.info("━━━ Starting pipeline for: %s ━━━", platform_name.upper())

    # Skip if already solved today
    if db.get_today_solved(platform_name):
        logger.info(
            "%s already solved today — skipping.", platform_name.upper()
        )
        return True   # count as success to avoid sending an error notification

    try:
        # Step 1 — Fetch the daily problem (blocking I/O → thread)
        logger.info("[%s] Fetching daily problem…", platform_name)
        problem: dict = await asyncio.to_thread(fetch_fn)
        problem["platform"] = platform_name   # normalise platform key

        logger.info(
            "[%s] Problem fetched: '%s'", platform_name, problem.get("title")
        )

        submit_fn = _SUBMITTERS.get(platform_name)
        solved: dict = {}
        verdict_info: dict = {}

        for attempt in range(1, 4):  # Up to 3 attempts (initial + 2 retries)
            # Step 2 — Solve with Claude (blocking API call → thread)
            logger.info("[%s] Solving problem (attempt %d/3)…", platform_name, attempt)
            solved = await asyncio.to_thread(solve_problem, problem)

            # Carry the URL forward for DB logging if the fetcher included it
            solved["url"] = problem.get("url") or problem.get("description_url")
            
            if not submit_fn:
                break
                
            # Step 2.5 - Submit to platform
            logger.info("[%s] Submitting to platform…", platform_name)
            try:
                verdict_info = await asyncio.to_thread(submit_fn, problem, solved["code"])
                if verdict_info.get("accepted"):
                    break
                logger.warning("[%s] Solution rejected on attempt %d: %s", platform_name, attempt, verdict_info.get("verdict"))
                problem["description"] += f"\n\nNOTE: A previous attempt in {solved['language']} failed with verdict: {verdict_info.get('verdict')}. Please provide a fixed, correct solution."
            except Exception as e:
                logger.warning("[%s] Submission error: %s", platform_name, e)
                verdict_info = {"accepted": False, "verdict": f"Submission Error: {e}"}
                break

        # Step 3 — Save solution to disk + DB (blocking file I/O → thread)
        logger.info("[%s] Saving solution…", platform_name)

        if verdict_info:
            solved["status"] = verdict_info.get("verdict", "Submitted")
            solved["submission_id"] = verdict_info.get("submission_id")
            
        file_path: str = await asyncio.to_thread(save_solution, solved)

        # Log submission result without crashing the pipeline
        if submit_fn:
            if verdict_info and verdict_info.get("accepted"):
                logger.info("[%s] Submission ACCEPTED ✅", platform_name.upper())
            elif verdict_info:
                verdict = verdict_info.get("verdict", "Unknown")
                logger.warning(
                    "[%s] Submission returned non-accepted verdict: %s — "
                    "solution saved locally, pipeline continuing.",
                    platform_name.upper(), verdict
                )
            else:
                logger.warning(
                    "[%s] No verdict received — submission may have failed silently. "
                    "Solution saved locally, pipeline continuing.",
                    platform_name.upper()
                )

        # Step 4 — Telegram success notification (always send if we got this far)
        notify_kwargs = {
            "platform": platform_name,
            "title": solved["problem_title"],
            "file_path": file_path,
            "code": solved["code"],
        }
        if verdict_info:
            notify_kwargs["verdict"] = verdict_info.get("verdict", "")
            notify_kwargs["submission_url"] = verdict_info.get("url", "")
            notify_kwargs["runtime"] = verdict_info.get("runtime", "")
            notify_kwargs["memory"] = verdict_info.get("memory", "")

        await asyncio.to_thread(
            telegram.send_solution_notification,
            **notify_kwargs
        )

        logger.info("━━━ %s DONE ✅ ━━━", platform_name.upper())
        return True

    except Exception as exc:
        logger.error(
            "[%s] Pipeline failed: %s", platform_name.upper(), exc, exc_info=True
        )
        # Telegram error notification — this itself must not raise
        try:
            await asyncio.to_thread(
                telegram.send_error_notification,
                platform_name,
                str(exc),
            )
        except Exception as notify_exc:
            logger.error(
                "Failed to send Telegram error notification: %s", notify_exc
            )
        return False


# ---------------------------------------------------------------------------
# Main orchestrator
# ---------------------------------------------------------------------------

async def run_daily_bot() -> None:
    """
    Run the complete daily coding challenge pipeline for all four platforms.

    Execution order:
        1. Config.validate()     — fail fast on missing .env keys
        2. db.init_db()          — create the SQLite table if absent
        3. For each platform:
              fetch → solve → save → notify
        4. Send a daily summary Telegram message

    Each platform runs sequentially to avoid hammering external services.
    If a platform fails, the error is caught, logged, and a Telegram alert is
    sent — the remaining platforms continue uninterrupted.

    Raises:
        ValueError: If Config.validate() fails (missing required .env keys).
                    This is intentionally NOT caught here so the scheduler
                    surfaces it immediately on startup.
    """
    logger.info("╔══════════════════════════════════════╗")
    logger.info("║    Daily Coding Challenge Bot        ║")
    logger.info("╚══════════════════════════════════════╝")
    logger.info("Bot started at: %s", date.today().isoformat())

    # Step 1 — Validate config (raises ValueError on missing keys)
    Config.validate()

    # Step 1.5 — Check cookie/token expiry and warn via Telegram
    try:
        await asyncio.to_thread(send_cookie_warnings)
    except Exception as e:
        logger.warning("Cookie expiry check failed: %s", e)

    # Step 2 — Ensure DB is ready
    db.init_db()

    # Platform registry — (name, fetch_function) pairs
    platforms: list[tuple[str, object]] = [
        ("leetcode",   lambda: lc_fetcher.fetch_daily_problem("leetcode")),
        ("leetcode_2", lambda: lc_fetcher.fetch_daily_problem("leetcode_2")),
        ("codeforces", cf_fetcher.fetch_daily_problem),
        ("codechef",   cc_fetcher.fetch_daily_problem),
        ("hackerrank", hr_fetcher.fetch_daily_problem),
    ]

    succeeded: list[str] = []
    failed: list[str] = []

    # Step 3 — Run each platform sequentially
    for platform_name, fetch_fn in platforms:
        # LeetCode requires fresh browser cookies that expire regularly.
        # Skip gracefully instead of failing with a cryptic 403 error.
        if platform_name.startswith("leetcode") and not Config.has_leetcode_credentials(platform_name):
            logger.warning(
                "[%s] Skipping — credentials for %s are not set or have expired.",
                platform_name.upper(), platform_name
            )
            try:
                await asyncio.to_thread(
                    telegram.send_error_notification,
                    platform_name,
                    f"Skipped: Credentials for {platform_name} are missing or expired. "
                    "Log into leetcode.com in your browser, copy the fresh cookies, "
                    "and update the environment variables.",
                )
            except Exception:
                pass
            failed.append(platform_name)
            continue


        ok: bool = await _run_platform(platform_name, fetch_fn)
        if ok:
            succeeded.append(platform_name)
        else:
            failed.append(platform_name)

    # Step 4 — Daily summary
    logger.info(
        "Daily bot completed — succeeded: %s | failed: %s",
        succeeded,
        failed,
    )
    await asyncio.to_thread(telegram.send_daily_summary, succeeded, failed)

    logger.info("╔══════════════════════════════════════╗")
    logger.info("║    Run Complete (%d/%d platforms)     ║",
                len(succeeded), len(platforms))
    logger.info("╚══════════════════════════════════════╝")


# ---------------------------------------------------------------------------
# CLI entry point
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    _configure_logging()
    asyncio.run(run_daily_bot())
