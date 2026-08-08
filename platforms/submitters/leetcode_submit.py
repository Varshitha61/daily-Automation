"""
platforms/submitters/leetcode_submit.py — LeetCode solution submitter.

Submits an AI-generated solution to LeetCode via the REST submit API using
the same session cookies as the fetcher. The submission will appear in the
user's profile, submission history, and activity heatmap.

Flow:
  1. POST solution to /problems/{slug}/submit/
  2. Poll /submissions/detail/{id}/check/ until verdict is returned
  3. Return { verdict, runtime, memory, submission_id, url }

Credentials required in .env:
    LEETCODE_SESSION    — value of the LEETCODE_SESSION browser cookie
    LEETCODE_CSRF_TOKEN — value of the csrftoken browser cookie
"""

import logging
import time
from typing import Optional

import requests

from config import Config

logger = logging.getLogger(__name__)

_SUBMIT_URL    = "https://leetcode.com/problems/{slug}/submit/"
_CHECK_URL     = "https://leetcode.com/submissions/detail/{id}/check/"
_SUBMISSION_URL = "https://leetcode.com/submissions/detail/{id}/"

# Language ID map — LeetCode uses short string identifiers
_LANG_MAP = {
    "cpp":     "cpp",
    "c++":     "cpp",
    "python":  "python3",
    "python3": "python3",
    "java":    "java",
    "c":       "c",
    "javascript": "javascript",
    "typescript": "typescript",
}

# Poll settings
_MAX_POLL_ATTEMPTS = 20
_POLL_INTERVAL_SEC = 3


def _build_session(platform: str = "leetcode") -> requests.Session:
    """Return an authenticated requests.Session for LeetCode."""
    session = requests.Session()
    if platform == "leetcode_2":
        session_cookie = Config.LEETCODE_2_SESSION
        csrf_token = Config.LEETCODE_2_CSRF_TOKEN
    else:
        session_cookie = Config.LEETCODE_SESSION
        csrf_token = Config.LEETCODE_CSRF_TOKEN

    session.cookies.set("LEETCODE_SESSION",  session_cookie,  domain="leetcode.com")
    session.cookies.set("csrftoken",         csrf_token, domain="leetcode.com")
    session.headers.update({
        "Content-Type":  "application/json",
        "X-CSRFToken":   csrf_token,
        "User-Agent": (
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
            "AppleWebKit/537.36 (KHTML, like Gecko) "
            "Chrome/124.0.0.0 Safari/537.36"
        ),
        "Referer": "https://leetcode.com",
    })
    return session



def submit_solution(problem: dict, code: str) -> dict:
    """
    Submit a solution to LeetCode and wait for the verdict.

    Args:
        problem (dict): Problem dict from leetcode fetcher.
                        Must contain: slug, question_id.
        code    (str):  The full solution source code.

    Returns:
        dict: {
            "verdict":       str  — e.g. "Accepted", "Wrong Answer", "Time Limit Exceeded"
            "runtime":       str  — e.g. "52 ms"  (or "" if not available)
            "memory":        str  — e.g. "45.2 MB" (or "" if not available)
            "submission_id": str  — numeric ID
            "url":           str  — full URL to the submission page
            "accepted":      bool — True if verdict == "Accepted"
        }

    Raises:
        RuntimeError: If submission fails or verdict cannot be retrieved.
    """
    slug        = problem.get("slug", "")
    question_id = str(problem.get("question_id", ""))
    lang        = _LANG_MAP.get(Config.SOLUTION_LANGUAGE.lower(), Config.SOLUTION_LANGUAGE)

    if not slug or not question_id:
        raise RuntimeError(
            f"LeetCode submitter: missing 'slug' or 'question_id' in problem dict: {problem}"
        )

    logger.info(
        "Submitting '%s' [id=%s] to LeetCode in %s...",
        problem.get("title", slug), question_id, lang,
    )
    
    # Add a delay to avoid rate limiting (429 Too Many Requests) 
    # especially during retries
    time.sleep(10)

    platform = problem.get("platform", "leetcode")
    session = _build_session(platform)


    # ── Step 1: POST the submission ──────────────────────────────────────────
    submit_url = _SUBMIT_URL.format(slug=slug)
    payload = {
        "lang":         lang,
        "question_id":  question_id,
        "typed_code":   code,
    }

    resp = session.post(submit_url, json=payload, timeout=30)
    if resp.status_code in (401, 403):
        raise RuntimeError(
            f"LeetCode submission rejected with HTTP {resp.status_code} — "
            "your LEETCODE_SESSION / LEETCODE_CSRF_TOKEN cookies have most likely "
            "expired. Log into leetcode.com in your browser, copy the fresh cookies, "
            "and update the GitHub Actions secrets."
        )
    if resp.status_code not in (200, 201):
        raise RuntimeError(
            f"LeetCode submission POST failed: {resp.status_code} — {resp.text[:300]}"
        )

    submission_id: Optional[str] = None
    try:
        submission_id = str(resp.json()["submission_id"])
    except (KeyError, ValueError) as exc:
        raise RuntimeError(
            f"LeetCode did not return a submission_id: {resp.text[:300]}"
        ) from exc

    logger.info("LeetCode submission created — id=%s, polling for verdict...", submission_id)

    # ── Step 2: Poll for verdict ─────────────────────────────────────────────
    check_url = _CHECK_URL.format(id=submission_id)
    for attempt in range(1, _MAX_POLL_ATTEMPTS + 1):
        time.sleep(_POLL_INTERVAL_SEC)
        check_resp = session.get(check_url, timeout=15)

        if check_resp.status_code != 200:
            logger.warning("Poll attempt %d returned HTTP %d", attempt, check_resp.status_code)
            continue

        data = check_resp.json()
        state = data.get("state", "")
        logger.debug("Poll attempt %d — state=%s", attempt, state)

        if state == "SUCCESS":
            # Map LeetCode status codes → human-readable strings
            status_map = {
                10: "Accepted",
                11: "Wrong Answer",
                12: "Memory Limit Exceeded",
                13: "Output Limit Exceeded",
                14: "Time Limit Exceeded",
                15: "Runtime Error",
                20: "Compile Error",
            }
            status_code   = data.get("status_code", 0)
            verdict       = status_map.get(status_code, data.get("status_msg", "Unknown"))
            runtime_raw   = data.get("status_runtime", "") or ""
            memory_raw    = data.get("status_memory",  "") or ""
            runtime       = runtime_raw if runtime_raw not in ("N/A", "null", "") else ""
            memory        = memory_raw  if memory_raw  not in ("N/A", "null", "") else ""
            accepted      = status_code == 10
            submission_url = _SUBMISSION_URL.format(id=submission_id)

            logger.info(
                "LeetCode verdict: %s | Runtime: %s | Memory: %s",
                verdict, runtime or "N/A", memory or "N/A",
            )

            return {
                "verdict":       verdict,
                "runtime":       runtime,
                "memory":        memory,
                "submission_id": submission_id,
                "url":           submission_url,
                "accepted":      accepted,
            }

        if state in ("PENDING", "STARTED"):
            continue   # still judging

        # Unexpected state
        logger.warning("Unexpected LeetCode submission state: %s", state)

    raise RuntimeError(
        f"LeetCode submission {submission_id} did not complete within "
        f"{_MAX_POLL_ATTEMPTS * _POLL_INTERVAL_SEC}s."
    )
