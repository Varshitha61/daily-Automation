"""
platforms/submitters/codeforces_submit.py — Codeforces solution submitter.

Submits an AI-generated solution to Codeforces via an authenticated HTTP POST
(no browser needed — Codeforces uses a simple form endpoint with CSRF token).

Flow:
  1. GET the submit page to extract the CSRF token
  2. POST the solution form to /problemset/submit
  3. Poll /api/user.status to get the verdict
  4. Return { verdict, submission_id, url, accepted }

Credentials required in .env:
    CODEFORCES_HANDLE     — your Codeforces username
    CODEFORCES_API_KEY    — Codeforces API key
    CODEFORCES_API_SECRET — Codeforces API secret
"""

import hashlib
import logging
import random
import re
import string
import time
from typing import Optional

import requests

from config import Config

logger = logging.getLogger(__name__)

_SUBMIT_PAGE_URL = "https://codeforces.com/problemset/submit"
_API_STATUS_URL  = "https://codeforces.com/api/user.status"

# Codeforces compiler IDs (programTypeId)
_LANG_MAP = {
    "cpp":    54,   # GNU G++17 7.3.0
    "c++":    54,
    "python": 31,   # Python 3
    "java":   36,   # Java 8
    "c":      43,   # GNU GCC C11 5.1.0
}

_MAX_POLL_ATTEMPTS = 25
_POLL_INTERVAL_SEC = 3

_USER_AGENT = (
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
    "AppleWebKit/537.36 (KHTML, like Gecko) "
    "Chrome/124.0.0.0 Safari/537.36"
)


def _make_api_sig(method: str, params: dict, secret: str) -> tuple[dict, str]:
    """Generate Codeforces API signature for authenticated endpoints."""
    rand = "".join(random.choices(string.ascii_lowercase + string.digits, k=6))
    params["apiKey"]  = Config.CODEFORCES_API_KEY
    params["time"]    = str(int(time.time()))
    params["apiSig"]  = "placeholder"  # temporary

    sorted_params = "&".join(f"{k}={v}" for k, v in sorted(params.items()) if k != "apiSig")
    to_hash = f"{rand}/{method}?{sorted_params}#{secret}"
    sig = rand + hashlib.sha512(to_hash.encode()).hexdigest()
    params["apiSig"] = sig
    return params


def submit_solution(problem: dict, code: str) -> dict:
    """
    Submit a solution to Codeforces and wait for the verdict.

    Args:
        problem (dict): Problem dict from codeforces fetcher.
                        Must contain: contest_id, index.
        code    (str):  Full solution source code.

    Returns:
        dict: {
            "verdict":       str  — e.g. "Accepted", "Wrong answer on test 1"
            "submission_id": str  — numeric submission ID
            "url":           str  — link to the submission
            "accepted":      bool — True if verdict == "OK" (Accepted)
            "runtime":       str  — e.g. "46 ms"
            "memory":        str  — e.g. "0 KB"
        }

    Raises:
        RuntimeError: If submission or verdict retrieval fails.
    """
    contest_id = str(problem.get("contest_id", ""))
    index      = str(problem.get("index", ""))
    title      = problem.get("title", "unknown")
    lang_id    = _LANG_MAP.get(Config.SOLUTION_LANGUAGE.lower(), 54)

    if not contest_id or not index:
        raise RuntimeError(
            f"Codeforces submitter: missing 'contest_id' or 'index' in problem dict: {problem}"
        )

    logger.info(
        "Submitting '%s' [%s%s] to Codeforces...", title, contest_id, index
    )

    session = requests.Session()
    session.headers["User-Agent"] = _USER_AGENT

    # ── Step 1: GET submit page to grab CSRF token ───────────────────────────
    page_resp = session.get(
        _SUBMIT_PAGE_URL,
        params={"action": "submitSolutionFormAction"},
        timeout=20,
    )
    page_resp.raise_for_status()

    csrf_match = re.search(r'name="csrf_token"\s+value="([^"]+)"', page_resp.text)
    if not csrf_match:
        raise RuntimeError("Codeforces: could not extract CSRF token from submit page.")
    csrf_token = csrf_match.group(1)

    # ── Step 2: POST the solution ────────────────────────────────────────────
    form_data = {
        "csrf_token":             csrf_token,
        "action":                 "submitSolutionFormAction",
        "submittedProblemCode":   f"{contest_id}{index}",
        "programTypeId":          str(lang_id),
        "source":                 code,
        "tabSize":                "4",
        "sourceFile":             "",
    }

    submit_resp = session.post(
        _SUBMIT_PAGE_URL,
        data=form_data,
        timeout=30,
        allow_redirects=True,
    )
    submit_resp.raise_for_status()

    # Check we landed on My Submissions or got a success indicator
    if "submission" not in submit_resp.url and "my-submissions" not in submit_resp.text:
        # Try to extract error message from page
        err_match = re.search(r'class="error[^"]*"[^>]*>([^<]+)<', submit_resp.text)
        err_msg = err_match.group(1).strip() if err_match else "unknown error"
        raise RuntimeError(f"Codeforces submission may have failed: {err_msg}")

    logger.info("Codeforces submission posted. Polling API for verdict...")

    # ── Step 3: Poll Codeforces API for verdict ──────────────────────────────
    # Give Codeforces a moment before polling
    time.sleep(5)

    submission_id: Optional[str] = None
    verdict:        str = "Pending"
    runtime:        str = ""
    memory_str:     str = ""
    accepted        = False

    for attempt in range(1, _MAX_POLL_ATTEMPTS + 1):
        try:
            params = {
                "handle": Config.CODEFORCES_HANDLE,
                "count":  "5",
            }
            api_resp = session.get(_API_STATUS_URL, params=params, timeout=15)
            api_resp.raise_for_status()
            data = api_resp.json()

            if data.get("status") != "OK":
                logger.warning("CF API returned status: %s", data.get("status"))
                time.sleep(_POLL_INTERVAL_SEC)
                continue

            submissions = data.get("result", [])
            for sub in submissions:
                prob = sub.get("problem", {})
                if (str(prob.get("contestId", "")) == contest_id and
                        prob.get("index", "") == index):
                    submission_id = str(sub.get("id", ""))
                    cf_verdict    = sub.get("verdict", "")

                    if cf_verdict in ("", "TESTING", None):
                        logger.debug("Attempt %d — still judging...", attempt)
                        break  # still being judged, keep polling

                    # Map CF verdict codes to human-readable
                    verdict_map = {
                        "OK":                   "Accepted",
                        "WRONG_ANSWER":         "Wrong Answer",
                        "TIME_LIMIT_EXCEEDED":  "Time Limit Exceeded",
                        "MEMORY_LIMIT_EXCEEDED":"Memory Limit Exceeded",
                        "RUNTIME_ERROR":        "Runtime Error",
                        "COMPILATION_ERROR":    "Compilation Error",
                        "SKIPPED":              "Skipped",
                        "REJECTED":             "Rejected",
                        "FAILED":               "Failed",
                    }
                    verdict  = verdict_map.get(cf_verdict, cf_verdict)
                    accepted = cf_verdict == "OK"

                    time_ms = sub.get("timeConsumedMillis", 0)
                    mem_kb  = sub.get("memoryConsumedBytes", 0) // 1024
                    runtime = f"{time_ms} ms"
                    memory_str = f"{mem_kb} KB"

                    sub_url = (
                        f"https://codeforces.com/contest/{contest_id}"
                        f"/submission/{submission_id}"
                    )

                    logger.info(
                        "Codeforces verdict: %s | Runtime: %s | Memory: %s",
                        verdict, runtime, memory_str,
                    )

                    return {
                        "verdict":       verdict,
                        "submission_id": submission_id,
                        "url":           sub_url,
                        "accepted":      accepted,
                        "runtime":       runtime,
                        "memory":        memory_str,
                    }

        except Exception as exc:
            logger.warning("CF API poll attempt %d failed: %s", attempt, exc)

        time.sleep(_POLL_INTERVAL_SEC)

    raise RuntimeError(
        f"Codeforces submission for '{title}' did not complete within "
        f"{_MAX_POLL_ATTEMPTS * _POLL_INTERVAL_SEC}s. "
        f"Last submission_id seen: {submission_id}"
    )
