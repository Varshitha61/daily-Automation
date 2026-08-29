"""
platforms/submitters/atcoder_submit.py — AtCoder solution submitter using Playwright.

Navigates to the contest's submit page, logs in if necessary (using credentials from
Config), selects the task and language, injects code into CodeMirror, submits, and polls
for the verdict.
"""

import json
import logging
import time
from pathlib import Path
from typing import Optional

from playwright.sync_api import (
    sync_playwright,
    Browser,
    BrowserContext,
    Page,
    TimeoutError as PWTimeout,
)

from config import Config

logger = logging.getLogger(__name__)

# Path to the persistent browser profile
_STORAGE_STATE_PATH: Path = (
    Path(__file__).parent.parent.parent / "logs" / "atcoder_session.json"
)

_LOGIN_URL: str = "https://atcoder.jp/login"
_BASE_URL: str = "https://atcoder.jp"
_WAIT_MS: int = 45_000


def _build_session_from_env() -> None:
    """
    Build the Playwright storage state JSON from ATCODER_SESSION in .env.
    Called before every submit so the latest cookie is always used.
    """
    if not Config.ATCODER_SESSION:
        return
    cookies = [
        {
            "name": "REVEL_SESSION",
            "value": Config.ATCODER_SESSION,
            "domain": "atcoder.jp",
            "path": "/",
            "expires": 1803529078.0,   # 2027-02-25
            "httpOnly": True,
            "secure": True,
            "sameSite": "Lax",
        },
        {
            "name": "REVEL_FLASH",
            "value": "",
            "domain": "atcoder.jp",
            "path": "/",
            "expires": -1,
            "httpOnly": True,
            "secure": True,
            "sameSite": "Lax",
        },
    ]
    state = {"cookies": cookies, "origins": []}
    _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
    _STORAGE_STATE_PATH.write_text(json.dumps(state, indent=2))
    logger.info("AtCoder session built from .env cookie.")


def _build_context(browser: Browser) -> BrowserContext:
    user_agent = (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/124.0.0.0 Safari/537.36"
    )

    if _STORAGE_STATE_PATH.exists():
        logger.info("Loading saved AtCoder session from %s", _STORAGE_STATE_PATH)
        return browser.new_context(
            storage_state=str(_STORAGE_STATE_PATH),
            user_agent=user_agent,
            permissions=["clipboard-read", "clipboard-write"],
        )

    logger.info("No saved AtCoder session — starting fresh context.")
    return browser.new_context(
        user_agent=user_agent,
        permissions=["clipboard-read", "clipboard-write"],
    )


def _is_logged_in(page: Page) -> bool:
    """Return True if the user is currently logged in (logout or user dropdown exists)."""
    try:
        # Check for logout link or username dropdown/link
        el = page.query_selector("a[href*='/logout'], .header-username, a[href*='/settings']")
        return el is not None
    except Exception:
        return False


def _login(page: Page) -> None:
    """
    Log in to AtCoder using credentials from Config.
    Raises RuntimeError if login fails.
    """
    logger.info("Logging in to AtCoder as '%s'…", Config.ATCODER_USERNAME)

    try:
        page.goto(_LOGIN_URL, timeout=_WAIT_MS, wait_until="domcontentloaded")

        # Already redirected or logged in
        if "login" not in page.url:
            logger.info("AtCoder: already logged in.")
            return

        # Wait for form inputs
        page.wait_for_selector("input[name='username']", timeout=15000)

        # Fill login form
        page.locator("input[name='username']").fill(Config.ATCODER_USERNAME)
        page.locator("input[name='password']").fill(Config.ATCODER_PASSWORD)
        page.click("button[type='submit']")

        try:
            page.wait_for_url(lambda url: "login" not in url, timeout=15000)
        except PWTimeout:
            pass

        if "login" in page.url:
            error_el = page.query_selector(".alert-danger, .error-message, .alert")
            error_text = error_el.inner_text().strip() if error_el else "Wrong credentials or login block"
            raise RuntimeError(
                f"AtCoder login failed — still on login page. Error: {error_text}"
            )

        logger.info("AtCoder login successful. URL: %s", page.url)

    except PWTimeout as exc:
        raise RuntimeError(f"AtCoder login timed out: {exc}") from exc


def _save_session(page: Page) -> None:
    """Persist current session cookies to disk for reuse."""
    try:
        _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
        page.context.storage_state(path=str(_STORAGE_STATE_PATH))
        logger.debug("AtCoder session saved to %s", _STORAGE_STATE_PATH)
    except Exception as e:
        logger.warning("Could not save AtCoder session: %s", e)


def submit_solution(problem: dict, code: str) -> dict:
    """
    Submit a solution to AtCoder via Playwright.

    Args:
        problem (dict): Must contain 'contest_id' and 'problem_id'.
        code    (str): Full source code to submit.

    Returns:
        dict: {
            "verdict": str,
            "accepted": bool,
            "submission_id": str,
            "url": str,
            "runtime": str,
            "memory": str
        }
    """
    contest_id = problem.get("contest_id")
    problem_id = problem.get("problem_id")
    if not contest_id or not problem_id:
        raise RuntimeError(
            f"AtCoder submitter: missing 'contest_id' or 'problem_id' in problem: {problem}"
        )

    submit_url = f"https://atcoder.jp/contests/{contest_id}/submit"
    # Build session file from cookie in .env before submitting
    _build_session_from_env()

    with sync_playwright() as p:
        browser = p.chromium.launch(
            headless=True,
            args=[
                "--disable-blink-features=AutomationControlled",
                "--no-sandbox",
                "--disable-setuid-sandbox",
                "--disable-dev-shm-usage",
                "--disable-gpu",
            ],
        )
        context = _build_context(browser)
        page = context.new_page()

        try:
            # ── 1. Navigate to contest submissions or home to verify session ──
            page.goto(_BASE_URL, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(2)

            if not _is_logged_in(page):
                _login(page)
                _save_session(page)
            else:
                logger.info("AtCoder: session is valid.")

            # ── 2. Open Submit page ──────────────────────────────────────────
            page.goto(submit_url, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(3)

            # If redirected to login again, log in
            if "login" in page.url:
                logger.warning("AtCoder session invalid on submit page. Logging in again...")
            # Navigate directly to home to verify cookies work
            page.goto(_BASE_URL, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(2)
            
            if not _is_logged_in(page):
                raise RuntimeError("Your ATCODER_SESSION cookie has expired. Please log into atcoder.jp, copy the new REVEL_SESSION cookie, and update your GitHub Actions Secrets.")

            # ── 2. Navigate to the problem page ───────────────────────────────────────────────
            try:
                page.wait_for_selector("select[name='data.TaskScreenName']", timeout=15000)
                # Find task value by matching substring
                task_val = page.evaluate("""(probId) => {
                    const select = document.querySelector("select[name='data.TaskScreenName']");
                    if (!select) return null;
                    const options = Array.from(select.options);
                    const target = probId.toLowerCase();
                    const opt = options.find(o => o.value.toLowerCase().includes(target) || o.text.toLowerCase().includes(target));
                    return opt ? opt.value : null;
                }""", problem_id)

                if not task_val:
                    raise RuntimeError(f"Could not find task {problem_id} in the dropdown.")

                page.select_option("select[name='data.TaskScreenName']", value=task_val)
                logger.info("Selected task option: %s", task_val)
            except Exception as e:
                raise RuntimeError(f"Failed to select task: {e}")

            # ── 4. Select Language ───────────────────────────────────────────
            try:
                page.wait_for_selector("select[name='data.LanguageId']", timeout=10000)
                lang_id = page.evaluate("""(langName) => {
                    const select = document.querySelector("select[name='data.LanguageId']");
                    if (!select) return null;
                    const options = Array.from(select.options);
                    const query = langName.toLowerCase();

                    // Standard mappings
                    if (query.includes("python") || query.includes("py")) {
                        // Prefer PyPy3 if available
                        const pypyOpt = options.find(opt => opt.text.toLowerCase().includes("pypy3") || opt.text.toLowerCase().includes("pypy 3"));
                        if (pypyOpt) return pypyOpt.value;
                        const pythonOpt = options.find(opt => opt.text.toLowerCase().includes("python3") || opt.text.toLowerCase().includes("python 3") || opt.text.toLowerCase().includes("python"));
                        if (pythonOpt) return pythonOpt.value;
                    }

                    if (query.includes("cpp") || query.includes("c++")) {
                        // Prefer GCC latest version
                        const cppOpt = options.find(opt => opt.text.toLowerCase().includes("c++") && opt.text.toLowerCase().includes("gcc"));
                        if (cppOpt) return cppOpt.value;
                        const cppAny = options.find(opt => opt.text.toLowerCase().includes("c++"));
                        if (cppAny) return cppAny.value;
                    }

                    if (query.includes("java")) {
                        const javaOpt = options.find(opt => opt.text.toLowerCase().includes("java"));
                        if (javaOpt) return javaOpt.value;
                    }

                    if (query.includes("javascript") || query.includes("js") || query.includes("node")) {
                        const jsOpt = options.find(opt => opt.text.toLowerCase().includes("javascript") || opt.text.toLowerCase().includes("node"));
                        if (jsOpt) return jsOpt.value;
                    }

                    // Fallback to generic substring match
                    const fallbackOpt = options.find(opt => opt.text.toLowerCase().includes(query));
                    if (fallbackOpt) return fallbackOpt.value;

                    return null;
                }""", Config.SOLUTION_LANGUAGE)

                if not lang_id:
                    raise RuntimeError(f"Could not match SOLUTION_LANGUAGE '{Config.SOLUTION_LANGUAGE}' in AtCoder languages.")

                page.select_option("select[name='data.LanguageId']", value=lang_id)
                logger.info("Selected language option: %s", lang_id)
            except Exception as e:
                raise RuntimeError(f"Failed to select language: {e}")

            # ── 5. Inject Code into CodeMirror ──────────────────────────────
            try:
                page.wait_for_selector(".CodeMirror, textarea[name='sourceCode']", timeout=15000)
                injected = page.evaluate("""(code) => {
                    const cm = document.querySelector('.CodeMirror');
                    if (cm && cm.CodeMirror) {
                        cm.CodeMirror.setValue(code);
                        return true;
                    }
                    const textarea = document.querySelector('textarea[name="sourceCode"]');
                    if (textarea) {
                        textarea.value = code;
                        return true;
                    }
                    return false;
                }""", code)

                if not injected:
                    # Fallback to copy-paste style if CodeMirror API didn't bind
                    textarea = page.locator("textarea[name='sourceCode']").first
                    textarea.click()
                    page.keyboard.press("Control+A")
                    page.keyboard.press("Backspace")
                    page.evaluate("async (text) => await navigator.clipboard.writeText(text)", code)
                    page.keyboard.press("Control+V")
                    time.sleep(1)

                logger.info("Successfully injected solution code.")
            except Exception as e:
                raise RuntimeError(f"Failed to inject solution code: {e}")

            # ── 6. Click Submit ──────────────────────────────────────────────
            try:
                page.click("button#submit, button[type='submit']")
                logger.info("Clicked Submit button.")
            except Exception as e:
                raise RuntimeError(f"Failed to click Submit button: {e}")

            # ── 7. Poll Verdict ──────────────────────────────────────────────
            submissions_url = f"https://atcoder.jp/contests/{contest_id}/submissions/me"
            logger.info("Navigating to submissions log to poll verdict: %s", submissions_url)

            # Max 20 poll attempts (each 3 seconds = 1 minute total)
            verdict_info = None
            for attempt in range(1, 21):
                page.goto(submissions_url, timeout=_WAIT_MS, wait_until="domcontentloaded")
                time.sleep(2)

                res = page.evaluate("""() => {
                    const table = document.querySelector("table");
                    if (!table) return null;
                    const firstRow = table.querySelector("tbody tr");
                    if (!firstRow) return null;

                    const cells = Array.from(firstRow.querySelectorAll("td"));
                    if (cells.length < 7) return null;

                    // Status is typically the 7th cell (index 6) or matches standard status strings
                    const statusCell = cells.find(td => {
                        const text = td.textContent.trim();
                        return ["WJ", "AC", "WA", "TLE", "MLE", "RE", "CE", "OLE", "IE", "QST", "WR"].includes(text) 
                               || td.querySelector("span.label") !== null;
                    }) || cells[6];

                    if (!statusCell) return null;

                    const verdict = statusCell.textContent.trim();

                    // If waiting for judging or empty, it's not ready
                    if (verdict === "WJ" || verdict === "" || verdict.includes("Judging")) {
                        return { ready: false };
                    }

                    // Extract detail link and submission ID
                    const detailLink = firstRow.querySelector("a[href*='/submissions/']");
                    const submissionId = detailLink ? detailLink.getAttribute("href").split("/").pop() : "unknown";

                    // Execution time (index 7)
                    const timeCell = cells[7];
                    const runtime = timeCell ? timeCell.textContent.trim() : "";

                    // Memory (index 8)
                    const memCell = cells[8];
                    const memory = memCell ? memCell.textContent.trim() : "";

                    return {
                        ready: true,
                        verdict: verdict,
                        submission_id: submissionId,
                        runtime: runtime,
                        memory: memory,
                        accepted: verdict === "AC"
                    };
                }""")

                if res and res.get("ready"):
                    verdict_info = res
                    break

                logger.info("Poll attempt %d: Verdict still judging (WJ) or table loading...", attempt)
                time.sleep(3)

            if not verdict_info:
                logger.warning("Timed out waiting for AtCoder submission verdict.")
                verdict_info = {
                    "verdict": "Unknown (Timed Out)",
                    "accepted": False,
                    "submission_id": "unknown",
                    "runtime": "",
                    "memory": "",
                }

            submission_id = verdict_info.get("submission_id", "unknown")
            sub_details_url = f"https://atcoder.jp/contests/{contest_id}/submissions/{submission_id}"

            result = {
                "verdict": verdict_info.get("verdict"),
                "accepted": verdict_info.get("accepted"),
                "submission_id": submission_id,
                "url": sub_details_url,
                "runtime": verdict_info.get("runtime"),
                "memory": verdict_info.get("memory"),
            }

            logger.info("AtCoder verdict parsed: %s", result)
            _save_session(page)
            return result

        finally:
            context.close()
            browser.close()
