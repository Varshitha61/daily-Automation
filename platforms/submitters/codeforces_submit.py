"""
platforms/submitters/codeforces_submit.py — Codeforces solution submitter.

Uses requests (not Playwright) to avoid Cloudflare bot detection.
Auth is done via session cookies (39ce7, JSESSIONID, X-User-Sha1, cf_clearance).
Verdict is polled via the official Codeforces API — no auth required.

Flow:
  1. GET the submit page with session cookies → extract CSRF token
  2. POST the submission form
  3. Poll https://codeforces.com/api/contest.status for verdict

Required GitHub Secrets:
    CODEFORCES_39CE7          — main session cookie (from browser DevTools)
    CODEFORCES_JSESSIONID     — Java session cookie
    CODEFORCES_X_USER_SHA1    — user identity cookie
    CODEFORCES_CF_CLEARANCE   — Cloudflare bypass cookie
    CODEFORCES_HANDLE         — your Codeforces handle (for API polling)
"""

import logging
import re
import time
from typing import Optional

import requests

from config import Config

logger = logging.getLogger(__name__)

_WAIT_MS: int = 60_000

# Codeforces compiler IDs
_LANG_MAP = {
    "cpp":     "73",   # GNU G++20 14.2 (current recommended)
    "c++":     "73",
    "python":  "31",   # Python 3.8.12
    "python3": "31",
    "java":    "36",   # Java 8
    "c":       "43",   # GNU GCC C11 5.1.0
}

_HEADERS = {
    "User-Agent": (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/124.0.0.0 Safari/537.36"
    ),
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
    "Accept-Language": "en-US,en;q=0.9",
    "Accept-Encoding": "gzip, deflate, br",
    "Connection": "keep-alive",
    "Upgrade-Insecure-Requests": "1",
}


# ---------------------------------------------------------------------------
# Session builder
# ---------------------------------------------------------------------------

def _build_session() -> requests.Session:
    """Build a requests.Session with all Codeforces auth cookies."""
    session = requests.Session()
    session.headers.update(_HEADERS)

    cookies = {
        "39ce7":       Config.CODEFORCES_39CE7.strip(),
        "JSESSIONID":  Config.CODEFORCES_JSESSIONID.strip(),
        "X-User-Sha1": Config.CODEFORCES_X_USER_SHA1.strip(),
    }
    if Config.CODEFORCES_CF_CLEARANCE.strip():
        cookies["cf_clearance"] = Config.CODEFORCES_CF_CLEARANCE.strip()

    for name, value in cookies.items():
        if value:
            session.cookies.set(name, value, domain=".codeforces.com")

    return session


# ---------------------------------------------------------------------------
# CSRF token extractor
# ---------------------------------------------------------------------------

def _get_csrf_token(session: requests.Session, url: str) -> str:
    """
    GET a Codeforces page and extract the CSRF token.
    Raises RuntimeError if blocked by Cloudflare or token not found.
    """
    logger.info("Fetching CSRF token from: %s", url)
    resp = session.get(url, timeout=30)

    # Check for Cloudflare block
    if "Just a moment" in resp.text or resp.status_code == 403:
        raise RuntimeError(
            "Cloudflare is blocking Codeforces requests. "
            "Your CODEFORCES_CF_CLEARANCE cookie has expired or is IP-bound. "
            "Log into codeforces.com in your browser, copy the new cf_clearance "
            "cookie from DevTools, and update the GitHub Secret CODEFORCES_CF_CLEARANCE."
        )

    # Check for redirect to login
    if "/enter" in resp.url or "login" in resp.url.lower():
        raise RuntimeError(
            "Codeforces session cookies are invalid or expired. "
            "Update CODEFORCES_39CE7 and CODEFORCES_JSESSIONID in GitHub Secrets."
        )

    # Extract CSRF token — it's in a <meta> tag or hidden form field
    patterns = [
        r'<meta name="X-Csrf-Token" content="([^"]+)"',
        r"csrf_token['\"]?\s*[:=]\s*['\"]([a-f0-9]{32})",
        r'name="csrf_token"\s+value="([^"]+)"',
        r'value="([a-f0-9]{32})"',
    ]
    for pattern in patterns:
        match = re.search(pattern, resp.text)
        if match:
            token = match.group(1)
            logger.debug("CSRF token found: %s…", token[:8])
            return token

    raise RuntimeError(
        f"Could not extract CSRF token from Codeforces page. "
        f"URL: {resp.url} | Status: {resp.status_code}"
    )


# ---------------------------------------------------------------------------
# Verdict polling via Codeforces API (no auth required)
# ---------------------------------------------------------------------------

def _poll_verdict_api(contest_id: str, index: str) -> dict:
    """
    Poll the Codeforces API for the latest submission verdict.

    Uses https://codeforces.com/api/contest.status which is public and
    requires no authentication — avoids Cloudflare entirely.
    """
    handle = Config.CODEFORCES_HANDLE
    api_url = (
        f"https://codeforces.com/api/contest.status"
        f"?contestId={contest_id}&handle={handle}&from=1&count=10"
    )
    logger.info("Polling verdict via API: %s", api_url)

    for attempt in range(1, 25):
        time.sleep(5)
        try:
            resp = requests.get(api_url, timeout=15)
            data = resp.json()

            if data.get("status") != "OK":
                logger.warning(
                    "Attempt %d — API returned status: %s", attempt, data.get("status")
                )
                continue

            submissions = data.get("result", [])
            if not submissions:
                logger.debug("Attempt %d — no submissions found yet.", attempt)
                continue

            # Find the most recent submission for this problem
            for sub in submissions:
                sub_index = sub.get("problem", {}).get("index", "")
                if sub_index.upper() != index.upper():
                    continue

                verdict = sub.get("verdict", "")
                if verdict in ("TESTING", ""):
                    logger.debug(
                        "Attempt %d — still judging: %s", attempt, verdict
                    )
                    break  # break inner loop, continue outer poll loop

                # Got a final verdict
                sub_id = str(sub.get("id", "unknown"))
                accepted = verdict == "OK"
                readable = "Accepted" if accepted else verdict.replace("_", " ").title()
                runtime = f"{sub.get('timeConsumedMillis', 0)} ms"
                memory  = f"{sub.get('memoryConsumedBytes', 0) // 1024} KB"
                sub_url = (
                    f"https://codeforces.com/contest/{contest_id}"
                    f"/submission/{sub_id}"
                )

                logger.info(
                    "Codeforces verdict: %s | id=%s | time=%s | mem=%s",
                    readable, sub_id, runtime, memory,
                )
                return {
                    "verdict":       readable,
                    "submission_id": sub_id,
                    "url":           sub_url,
                    "accepted":      accepted,
                    "runtime":       runtime,
                    "memory":        memory,
                }

        except Exception as e:
            logger.warning("Attempt %d — API error: %s", attempt, e)

    logger.warning("Verdict not received within timeout — returning Pending.")
    return {
        "verdict":       "Pending (timed out)",
        "submission_id": "unknown",
        "url":           "",
        "accepted":      False,
        "runtime":       "",
        "memory":        "",
    }


# ---------------------------------------------------------------------------
# Main submit function
# ---------------------------------------------------------------------------

def submit_solution(problem: dict, code: str) -> dict:
    """
    Submit a solution to Codeforces via HTTP requests (no Playwright).

    Args:
        problem: Must contain 'contest_id' and 'index'.
        code:    Full source code string.

    Returns:
        dict with keys: verdict, submission_id, url, accepted, runtime, memory.
    """
    contest_id = str(problem.get("contest_id", ""))
    index      = str(problem.get("index", ""))
    title      = problem.get("title", "unknown")
    lang_id    = _LANG_MAP.get(Config.SOLUTION_LANGUAGE.lower(), "73")

    if not contest_id or not index:
        raise RuntimeError(
            f"Codeforces submitter: 'contest_id' or 'index' missing in: {problem}"
        )

    logger.info(
        "Submitting '%s' [%s%s] to Codeforces (lang=%s)…",
        title, contest_id, index, lang_id,
    )

    session = _build_session()

    # ── Step 1: GET the submit page to get CSRF token ────────────────────────
    submit_url = f"https://codeforces.com/contest/{contest_id}/submit"
    csrf_token = _get_csrf_token(session, submit_url)

    # ── Step 2: POST the submission ─────────────────────────────────────────
    session.headers.update({
        "Referer": submit_url,
        "Content-Type": "application/x-www-form-urlencoded",
        "Origin": "https://codeforces.com",
    })

    payload = {
        "csrf_token":            csrf_token,
        "action":                "submitSolutionFormSubmitted",
        "contestId":             contest_id,
        "submittedProblemIndex": index,
        "programTypeId":         lang_id,
        "source":                code,
        "tabSize":               "4",
        "sourceFile":            "",
        "_tta":                  "176",
    }

    logger.info("POSTing submission to Codeforces…")
    post_resp = session.post(submit_url, data=payload, timeout=30, allow_redirects=True)

    logger.info(
        "Submit response: HTTP %d | URL: %s",
        post_resp.status_code, post_resp.url,
    )

    # Check for common errors
    if post_resp.status_code == 403 or "Just a moment" in post_resp.text:
        raise RuntimeError(
            "Cloudflare blocked the Codeforces submission POST. "
            "CODEFORCES_CF_CLEARANCE is expired or IP-bound. "
            "Refresh it from your browser and update the GitHub Secret."
        )

    if "You have submitted exactly the same code before" in post_resp.text:
        logger.warning("Codeforces rejected: duplicate submission.")
        # Still poll for the previous verdict
    elif post_resp.status_code not in (200, 201, 302):
        raise RuntimeError(
            f"Codeforces submission POST failed with HTTP {post_resp.status_code}."
        )

    logger.info("Submission sent — waiting 8s before polling verdict…")
    time.sleep(8)

    # ── Step 3: Poll verdict via Codeforces API ──────────────────────────────
    return _poll_verdict_api(contest_id, index)
