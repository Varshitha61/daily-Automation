"""
platforms/submitters/codechef_submit.py — CodeChef solution submitter using Playwright.

Uses cookie-based authentication (same as the fetcher) to navigate to the
problem page, inject code into the Monaco editor, and submit.

Required GitHub Secrets (cookie-based — refresh when expired):
    CODECHEF_AUTH_TOKEN   — Authorization cookie value
    CODECHEF_SESSION      — SESS... cookie value
    CODECHEF_CF_CLEARANCE — cf_clearance cookie value
    CODECHEF_UID          — uid cookie value
    CODECHEF_USERKEY      — user_key cookie value
"""

import logging
import time
from pathlib import Path

from playwright.sync_api import (
    sync_playwright,
    Browser,
    BrowserContext,
    Page,
    TimeoutError as PWTimeout,
)

from config import Config
from platforms.codechef import _build_session_from_env, _STORAGE_STATE_PATH

logger = logging.getLogger(__name__)

_BASE_URL: str = "https://www.codechef.com"
_WAIT_MS: int = 45_000


def _build_context(browser: Browser) -> BrowserContext:
    user_agent = (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/124.0.0.0 Safari/537.36"
    )

    if _STORAGE_STATE_PATH.exists():
        logger.info("Loading saved CodeChef session from %s", _STORAGE_STATE_PATH)
        return browser.new_context(
            storage_state=str(_STORAGE_STATE_PATH),
            user_agent=user_agent,
            permissions=["clipboard-read", "clipboard-write"],
        )

    logger.info("No saved CodeChef session — starting fresh context.")
    return browser.new_context(
        user_agent=user_agent,
        permissions=["clipboard-read", "clipboard-write"],
    )


def _is_logged_in(page: Page) -> bool:
    """
    Return True if the CodeChef page shows a logged-in user.
    Uses multiple selector strategies to handle CodeChef's React UI.
    """
    try:
        # Try cookie-based check first (most reliable)
        cookies = page.context.cookies()
        auth_cookie = next(
            (c for c in cookies if c["name"] == "Authorization" and c["value"]),
            None
        )
        if auth_cookie:
            logger.debug("CodeChef: auth cookie present — assuming logged in.")
            return True

        # Fall back to DOM selectors
        el = page.query_selector(
            "a[href*='/users/'], "
            "button[class*='avatar'], "
            "img[class*='avatar'], "
            "[data-username], "
            "a[href='/logout']"
        )
        return el is not None
    except Exception:
        return False


def _save_session(page: Page) -> None:
    """Persist current session cookies to disk for reuse."""
    try:
        _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
        page.context.storage_state(path=str(_STORAGE_STATE_PATH))
        logger.debug("CodeChef session saved to %s", _STORAGE_STATE_PATH)
    except Exception as e:
        logger.warning("Could not save CodeChef session: %s", e)


def submit_solution(problem: dict, code: str) -> dict:
    """
    Submit a solution to CodeChef via Playwright.

    Uses cookie-based auth (CODECHEF_AUTH_TOKEN etc.) — same as the fetcher.
    No username/password login is attempted.

    Args:
        problem (dict): Must contain 'url' (the problem page URL).
        code    (str):  Full source code to submit.

    Returns:
        dict: {
            "verdict": str,
            "accepted": bool,
            "url": str
        }
    """
    url = problem.get("url")
    if not url:
        raise RuntimeError("CodeChef submitter: missing 'url' in problem dict.")

    logger.info("Submitting CodeChef solution to %s", url)

    # Rebuild session from .env cookies before submitting
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
            # ── 1. Navigate to home and verify cookies ──────────────────────
            page.goto(_BASE_URL, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(3)

            if not _is_logged_in(page):
                raise RuntimeError(
                    "CodeChef cookies (CODECHEF_AUTH_TOKEN, CODECHEF_SESSION, etc.) "
                    "have expired or are invalid. Please log into codechef.com in your "
                    "browser, copy the new cookies, and update your GitHub Actions Secrets."
                )

            logger.info("CodeChef: session is valid, proceeding to submit.")

            # ── 2. Navigate to the problem page ────────────────────────────
            page.goto(url, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(3)

            # ── 3. Click "Solve" or "Submit" button to open editor ─────────
            try:
                page.wait_for_selector(
                    "button:has-text('Submit'), button:has-text('Solve')",
                    timeout=15_000
                )
                # Prefer "Solve" first (opens submission panel)
                solve_btn = page.locator("button:has-text('Solve')").first
                if solve_btn.count() > 0:
                    solve_btn.click()
                    time.sleep(2)
                    logger.info("Clicked 'Solve' button to open editor.")
            except PWTimeout:
                logger.warning("No Submit/Solve button found on CodeChef problem page — trying to submit directly.")

            # ── 4. Inject code into Monaco editor ─────────────────────────
            time.sleep(2)  # let Monaco load
            injected = page.evaluate("""(code) => {
                if (typeof monaco !== 'undefined' && monaco.editor.getModels().length > 0) {
                    monaco.editor.getModels()[0].setValue(code);
                    return true;
                }
                return false;
            }""", code)

            if not injected:
                logger.warning("Monaco not found — trying textarea/inputarea fallback.")
                textarea = page.locator(".inputarea").first
                if textarea.count() > 0:
                    textarea.click()
                    page.keyboard.press("Control+A")
                    page.keyboard.press("Backspace")
                    page.evaluate("async (text) => await navigator.clipboard.writeText(text)", code)
                    page.keyboard.press("Control+V")
                    time.sleep(1)
                    logger.info("Code injected via clipboard fallback.")
                else:
                    logger.warning("Could not find CodeChef editor — submission may fail.")

            time.sleep(1)

            # ── 5. Click the Submit button ─────────────────────────────────
            try:
                submit_btn = page.locator("button:has-text('Submit')").last
                submit_btn.wait_for(timeout=10_000)
                submit_btn.click()
                logger.info("Clicked CodeChef Submit button.")
            except Exception as e:
                raise RuntimeError(f"Could not click CodeChef Submit button: {e}")

            # ── 6. Wait for verdict ────────────────────────────────────────
            verdict = "Submitted"
            accepted = False

            try:
                status_locator = (
                    page.locator("text='Correct Answer'")
                    .or_(page.locator("text='Wrong Answer'"))
                    .or_(page.locator("text='Time Limit Exceeded'"))
                    .or_(page.locator("text='Compilation Error'"))
                    .or_(page.locator("text='Runtime Error'"))
                    .or_(page.locator("[class*='verdict'], [class*='result-verdict']"))
                )
                status_locator.first.wait_for(timeout=60_000)
                verdict = status_locator.first.inner_text().strip()
                accepted = "Correct Answer" in verdict
                logger.info("CodeChef verdict: %s", verdict)
            except PWTimeout:
                logger.warning("Timed out waiting for verdict on CodeChef.")
                verdict = "Unknown Verdict (Timed Out)"

            _save_session(page)

            return {
                "verdict": verdict,
                "accepted": accepted,
                "url": page.url,
            }

        finally:
            context.close()
            browser.close()
