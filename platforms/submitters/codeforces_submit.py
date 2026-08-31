"""
platforms/submitters/codeforces_submit.py — Codeforces solution submitter.

Uses a pre-authenticated browser session (Playwright storage state JSON) to
submit solutions — bypassing the Cloudflare-protected login page entirely.

WHY COOKIE-BASED AUTH?
Codeforces uses Cloudflare bot protection on its login page.  A headless
Playwright browser running in GitHub Actions (a datacenter IP) is instantly
blocked before it can even see the login form.  The workaround is to:
  1. Log in once in your REAL browser (Chrome / Firefox)
  2. Export the cookies via the helper script (export_cf_cookies.py)
  3. Store the JSON in the GitHub Secret CF_COOKIES_JSON

Flow:
  1. Load the cookie JSON from CF_COOKIES_JSON env var → inject into context
  2. Navigate directly to the problem submit page
  3. Select language, inject code, click Submit
  4. Poll https://codeforces.com/submissions/{handle} for verdict

Required GitHub Secrets:
    CF_COOKIES_JSON     — Playwright storage-state JSON (see export_cf_cookies.py)
    CODEFORCES_HANDLE   — your handle (used to poll your own submissions)
    CODEFORCES_PASSWORD — only used in the local fallback login path
"""

import json
import logging
import os
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

# Persistent session file (used when running locally)
_STORAGE_STATE_PATH: Path = (
    Path(__file__).parent.parent.parent / "logs" / "codeforces_session.json"
)

_WAIT_MS: int = 60_000

# Codeforces compiler IDs — keep in sync with what the site accepts
# Check current IDs at: https://codeforces.com/problemset/submit
_LANG_MAP = {
    "cpp":     "61",   # GNU G++17 9.2.0 (current default)
    "c++":     "61",
    "python":  "31",   # Python 3.8.12
    "python3": "31",
    "java":    "36",   # Java 8
    "c":       "43",   # GNU GCC C11 5.1.0
}


# ---------------------------------------------------------------------------
# Browser helpers
# ---------------------------------------------------------------------------

def _launch_browser(p) -> Browser:
    """Chromium with stealth-friendly flags. Runs non-headless locally to bypass Cloudflare."""
    is_ci = os.getenv("GITHUB_ACTIONS") == "true" or os.getenv("CI") == "true"
    return p.chromium.launch(
        headless=is_ci,
        args=[
            "--disable-blink-features=AutomationControlled",
            "--no-sandbox",
            "--disable-setuid-sandbox",
            "--disable-dev-shm-usage",
            "--disable-gpu",
            "--window-size=1280,800",
        ],
    )


def _build_context(browser: Browser) -> BrowserContext:
    """
    Create a BrowserContext, injecting cookies from one of three sources
    (highest → lowest priority):

    1. CF_COOKIES_JSON env var  — set this in GitHub Actions secrets
    2. Local session file       — saved after a successful form login
    3. Fresh context            — no cookies at all (login page will be shown)
    """
    ua = (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/124.0.0.0 Safari/537.36"
    )
    base_kwargs = dict(
        user_agent=ua,
        viewport={"width": 1280, "height": 800},
        locale="en-US",
    )

    # Priority 0.5: Direct env cookies
    cf_39ce7 = Config.CODEFORCES_39CE7.strip()
    cf_jsessionid = Config.CODEFORCES_JSESSIONID.strip()
    cf_xuser = Config.CODEFORCES_X_USER_SHA1.strip()
    if cf_39ce7:
        logger.info("Building Codeforces context using direct cookies from .env variables.")
        context = browser.new_context(**base_kwargs)
        cookies = [
            {
                "name": "39ce7",
                "value": cf_39ce7,
                "domain": ".codeforces.com",
                "path": "/",
                "httpOnly": True,
                "secure": True,
                "sameSite": "Lax"
            }
        ]
        if cf_jsessionid:
            cookies.append({
                "name": "JSESSIONID",
                "value": cf_jsessionid,
                "domain": "codeforces.com",
                "path": "/",
                "httpOnly": True,
                "secure": True,
                "sameSite": "Lax"
            })
        if cf_xuser:
            cookies.append({
                "name": "X-User-Sha1",
                "value": cf_xuser,
                "domain": ".codeforces.com",
                "path": "/",
                "httpOnly": False,
                "secure": True,
                "sameSite": "Lax"
            })
        context.add_cookies(cookies)
        return context


    # Priority 1: GitHub secret
    raw = os.getenv("CF_COOKIES_JSON", "").strip()
    if raw:
        logger.info("Loading Codeforces cookies from CF_COOKIES_JSON env var.")
        try:
            state = json.loads(raw)
            return browser.new_context(storage_state=state, **base_kwargs)
        except Exception as e:
            logger.warning("Failed to parse CF_COOKIES_JSON: %s", e)

    # Priority 2: local session file
    if _STORAGE_STATE_PATH.exists():
        logger.info("Loading saved Codeforces session from %s", _STORAGE_STATE_PATH)
        return browser.new_context(
            storage_state=str(_STORAGE_STATE_PATH), **base_kwargs
        )

    # Priority 3: fresh (will need to log in via form)
    logger.info("No saved Codeforces session found — will attempt form login.")
    return browser.new_context(**base_kwargs)


# ---------------------------------------------------------------------------
# Session verification & form login (fallback)
# ---------------------------------------------------------------------------

def _is_logged_in(page: Page) -> bool:
    """Return True if the page shows the user's handle (i.e. logged in)."""
    handle = Config.CODEFORCES_HANDLE.lower()
    try:
        page.wait_for_selector(
            f"a[href*='/profile/{handle}'], "
            f"a[href*='/profile/{Config.CODEFORCES_HANDLE}']",
            timeout=5_000,
        )
        return True
    except PWTimeout:
        return False


def _verify_or_login(page: Page) -> None:
    """
    Check whether the injected cookies are valid by visiting the home page.
    If not logged in, attempt the form-based login (works locally / on
    non-datacenter IPs).
    """
    page.goto("https://codeforces.com/", timeout=_WAIT_MS, wait_until="domcontentloaded")
    time.sleep(2)

    if _is_logged_in(page):
        logger.info("Codeforces: session valid — logged in as '%s'.", Config.CODEFORCES_HANDLE)
        return

    logger.warning(
        "Codeforces: injected cookies are expired or missing. "
        "Attempting form login (may fail in GitHub Actions due to Cloudflare)."
    )
    _form_login(page)


def _form_login(page: Page) -> None:
    """
    Form-based login — only works from non-datacenter IPs.
    In GitHub Actions the Cloudflare challenge will block this; use
    CF_COOKIES_JSON instead.
    """
    page.goto(
        "https://codeforces.com/enter?back=%2F",
        timeout=_WAIT_MS,
        wait_until="domcontentloaded",
    )
    time.sleep(3)

    if _is_logged_in(page):
        logger.info("Already logged in after redirect.")
        return

    # Check for Cloudflare block
    page_title = page.title() or ""
    page_html = page.content().lower()
    if "Just a moment" in page_title or "cloudflare" in page_html or "challenge" in page_html:
        raise RuntimeError(
            "Cloudflare is blocking Codeforces login. "
            "Your Codeforces cookies (CODEFORCES_39CE7, CODEFORCES_JSESSIONID) have expired. "
            "Please log into Codeforces in your browser, copy the new cookies, and update your GitHub Actions Secrets."
        )

    try:
        page.wait_for_selector("input#handleOrEmail", timeout=15_000)
    except PWTimeout:
        current_title = page.title() or ""
        current_html = page.content().lower()
        if "cloudflare" in current_html or "challenge" in current_html or "Just a moment" in current_title:
            raise RuntimeError(
                "Cloudflare is blocking Codeforces login. "
                "Your Codeforces cookies (CODEFORCES_39CE7, CODEFORCES_JSESSIONID) have expired. "
                "Please log into Codeforces in your browser, copy the new cookies, and update your GitHub Actions Secrets."
            )
        snippet = page.content()[:400]
        raise RuntimeError(
            f"Login form not found (possibly Cloudflare block or UI change). URL={page.url}  snippet={snippet}"
        )

    page.fill("input#handleOrEmail", Config.CODEFORCES_HANDLE)
    page.fill("input#password", Config.CODEFORCES_PASSWORD)
    page.click("input[type='submit'][value='Login']")

    try:
        page.wait_for_selector(
            f"a[href*='/profile/{Config.CODEFORCES_HANDLE.lower()}']",
            timeout=15_000,
        )
        logger.info("Codeforces form login successful.")
    except PWTimeout:
        snippet = page.content()[:400]
        raise RuntimeError(
            f"Login failed — handle link not found. URL={page.url}  snippet={snippet}"
        )

    # Save session for next local run
    _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
    page.context.storage_state(path=str(_STORAGE_STATE_PATH))
    logger.info("Session saved to %s", _STORAGE_STATE_PATH)


# ---------------------------------------------------------------------------
# Code injection
# ---------------------------------------------------------------------------

def _inject_code(page: Page, code: str) -> bool:
    """Try multiple strategies to fill the code into the textarea."""
    # 1. Standard Playwright fill
    try:
        ta = page.wait_for_selector("textarea#sourceCodeTextarea", timeout=8_000)
        ta.click()
        ta.fill(code)
        if ta.input_value() and len(ta.input_value()) > 5:
            logger.debug("Code injected via direct fill.")
            return True
    except Exception as e:
        logger.debug("Direct fill failed: %s", e)

    # 2. JS assignment
    try:
        page.evaluate(
            """(code) => {
                const ta = document.getElementById('sourceCodeTextarea');
                if (!ta) throw new Error('textarea not found');
                ta.value = code;
                ta.dispatchEvent(new Event('input',  {bubbles: true}));
                ta.dispatchEvent(new Event('change', {bubbles: true}));
            }""",
            code,
        )
        logger.debug("Code injected via JS.")
        return True
    except Exception as e:
        logger.debug("JS injection failed: %s", e)

    return False


# ---------------------------------------------------------------------------
# Verdict polling  (uses /submissions/{handle} — always shows YOUR submissions)
# ---------------------------------------------------------------------------

def _poll_verdict(page: Page, contest_id: str, index: str) -> dict:
    """
    Poll https://codeforces.com/submissions/{handle} for the latest
    submission matching this contest/problem.

    This URL always shows ONLY the logged-in user's own submissions,
    unlike /problemset/status which shows everyone's.

    Returns dict with verdict, submission_id, url, accepted, runtime, memory.
    """
    handle = Config.CODEFORCES_HANDLE
    # User submissions page — reliably shows OUR OWN submissions
    submissions_url = f"https://codeforces.com/submissions/{handle}"

    logger.info("Polling submissions at: %s", submissions_url)
    page.goto(submissions_url, timeout=_WAIT_MS, wait_until="domcontentloaded")
    time.sleep(2)

    submission_id: str = "unknown"
    verdict:       str = "Pending"
    accepted:      bool = False
    runtime:       str = ""
    memory_str:    str = ""
    sub_url:       str = ""

    for attempt in range(1, 30):
        try:
            # Find ALL submission rows for this exact problem
            rows = page.locator("table.status-frame-datatable tr[data-submission-id]")
            count = rows.count()

            if count == 0:
                logger.debug("Attempt %d — no submission rows found yet.", attempt)
                time.sleep(3)
                page.reload()
                continue

            # Find the row matching our contest+index (most recent one)
            target_row = None
            for i in range(min(count, 10)):  # check top 10 rows
                row = rows.nth(i)
                try:
                    problem_cell = row.locator("td").nth(3).inner_text().strip()
                    # Problem cell looks like "1234A" or "Contest Name - A"
                    if contest_id in problem_cell or index in problem_cell:
                        target_row = row
                        break
                except Exception:
                    continue

            # Fallback: use the very first (most recent) row
            if target_row is None:
                logger.debug("Attempt %d — no row matched %s%s, using first row.", attempt, contest_id, index)
                target_row = rows.first

            submission_id = target_row.get_attribute("data-submission-id") or "unknown"
            sub_url = (
                f"https://codeforces.com/contest/{contest_id}"
                f"/submission/{submission_id}"
            )

            verdict_text = target_row.locator("td.status-verdict-cell").inner_text().strip()

            if not verdict_text or any(
                s in verdict_text
                for s in ("Running on", "In queue", "Pending", "Judging", "Testing")
            ):
                logger.debug("Attempt %d — not ready: '%s'", attempt, verdict_text)
                time.sleep(4)
                page.reload()
                continue

            verdict  = verdict_text.splitlines()[0] if verdict_text else "Unknown"
            accepted = "Accepted" in verdict

            try:
                runtime    = target_row.locator("td.time-consumed-cell").inner_text().strip()
                memory_str = target_row.locator("td.memory-consumed-cell").inner_text().strip()
            except Exception:
                pass

            logger.info(
                "Codeforces verdict: %s | id=%s | time=%s | mem=%s",
                verdict, submission_id, runtime or "N/A", memory_str or "N/A",
            )
            break

        except Exception as e:
            logger.warning("Attempt %d — error reading verdict: %s", attempt, e)
            time.sleep(4)
            page.reload()

    return {
        "verdict":       verdict,
        "submission_id": submission_id,
        "url":           sub_url,
        "accepted":      accepted,
        "runtime":       runtime,
        "memory":        memory_str,
    }


# ---------------------------------------------------------------------------
# Main submit function
# ---------------------------------------------------------------------------

def submit_solution(problem: dict, code: str) -> dict:
    """
    Submit a solution to Codeforces.

    Args:
        problem: Must contain 'contest_id' and 'index'.
        code:    Full source code.

    Returns:
        dict with keys: verdict, submission_id, url, accepted, runtime, memory.
    """
    contest_id = str(problem.get("contest_id", ""))
    index      = str(problem.get("index", ""))
    title      = problem.get("title", "unknown")
    lang_id    = _LANG_MAP.get(Config.SOLUTION_LANGUAGE.lower(), "54")

    if not contest_id or not index:
        raise RuntimeError(
            f"Codeforces submitter: 'contest_id' or 'index' missing in: {problem}"
        )

    logger.info(
        "Submitting '%s' [%s%s] to Codeforces (lang=%s)...",
        title, contest_id, index, lang_id,
    )

    # ── Submit URL: use the problem-specific contest submit page.
    # This is more reliable than /problemset/submit for correctly
    # pre-populating the problem index field.
    # Try the contest-specific URL first; fall back to problemset URL.
    submit_url = f"https://codeforces.com/contest/{contest_id}/submit/{index}"

    with sync_playwright() as p:
        browser = _launch_browser(p)
        context = _build_context(browser)
        page    = context.new_page()

        try:
            # ── 1. Go directly to problem-specific submit page ──────────────
            logger.info("Opening submit page: %s", submit_url)
            page.goto(submit_url, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(3)

            # Check if Cloudflare is blocking us
            if "Just a moment" in (page.title() or ""):
                raise RuntimeError(
                    "Cloudflare blocked the submit page. "
                    "CF_COOKIES_JSON cookies may have expired or are IP-bound. "
                    "Update CF_COOKIES_JSON with fresh cookies from your browser.\n"
                    "Run: python export_cf_cookies.py"
                )

            # Check if we need to log in (redirected to /enter)
            if "/enter" in page.url or "login" in page.url.lower():
                logger.warning("Session cookies invalid — redirected to login. Attempting form login...")
                _form_login(page)
                page.goto(submit_url, timeout=_WAIT_MS, wait_until="domcontentloaded")
                time.sleep(3)

            # If the contest-specific URL didn't work, fall back to problemset/submit
            if "submit" not in page.url and "enter" not in page.url:
                logger.warning("Contest submit URL didn't work, trying problemset/submit...")
                fallback_url = (
                    f"https://codeforces.com/problemset/submit"
                    f"?contestId={contest_id}&submittedProblemIndex={index}"
                )
                page.goto(fallback_url, timeout=_WAIT_MS, wait_until="domcontentloaded")
                time.sleep(3)

            # Verify the submit form is present
            try:
                page.wait_for_selector(
                    "input[type='submit'][value='Submit'], form#submitForm, #pageContent form",
                    timeout=10_000
                )
                logger.info("Submit form found. Current URL: %s", page.url)
            except PWTimeout:
                logger.warning("Submit form not found. URL: %s", page.url)
                # Log page title to help debug
                logger.warning("Page title: %s", page.title())

            # ── 2. Fill in contested problem index (in case form needs it) ──
            try:
                problem_index_input = page.locator("input[name='submittedProblemIndex']")
                if problem_index_input.count() > 0:
                    problem_index_input.fill(index)
                    logger.debug("Filled problem index: %s", index)
            except Exception:
                pass

            # ── 3. Select language ──────────────────────────────────────────
            try:
                page.wait_for_selector("select[name='programTypeId']", timeout=10_000)
                page.select_option("select[name='programTypeId']", value=lang_id)
                logger.debug("Language set to id=%s", lang_id)
            except Exception as e:
                logger.warning("Language selector not found: %s", e)

            # ── 4. Disable Monaco editor toggle if present ──────────────────
            try:
                toggle = page.locator("a#toggleEditorCheckbox")
                if toggle.count() > 0 and "toggle-on" in (toggle.get_attribute("class") or ""):
                    toggle.click()
                    time.sleep(1)
            except Exception:
                pass

            # ── 5. Inject code ──────────────────────────────────────────────
            if not _inject_code(page, code):
                raise RuntimeError(
                    "Could not inject code into the Codeforces editor. "
                    "Check the page HTML and update _inject_code() selectors."
                )

            # ── 6. Submit ───────────────────────────────────────────────────
            logger.info("Clicking Submit button...")
            try:
                page.click("input[type='submit'][value='Submit']")
            except Exception:
                # Try alternate selector
                page.click("button[type='submit']")

            # Wait for page to navigate away from the submit form
            try:
                page.wait_for_url(lambda url: "submit" not in url.split("?")[0], timeout=15_000)
                logger.info("Navigated after submit. URL: %s", page.url)
            except PWTimeout:
                logger.warning("No navigation after submit detected. URL: %s", page.url)

            time.sleep(5)  # give Codeforces time to register the submission

            # ── 7. Poll verdict via /submissions/{handle} ───────────────────
            result = _poll_verdict(page, contest_id, index)

            # Persist session for next local run
            try:
                _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
                page.context.storage_state(path=str(_STORAGE_STATE_PATH))
            except Exception:
                pass

            return result

        finally:
            context.close()
            browser.close()
