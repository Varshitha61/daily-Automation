"""
platforms/submitters/codechef_submit.py — CodeChef solution submitter using Playwright.

Auth strategy (tries in order):
  1. Username + Password login (CODECHEF_USERNAME / CODECHEF_PASSWORD) — primary
  2. Cookie-based auth fallback (CODECHEF_AUTH_TOKEN etc.) — if credentials missing

Flow:
  1. Launch Playwright browser
  2. Log in via credentials (or inject cookies as fallback)
  3. Navigate to problem URL
  4. Click Solve → inject code → Submit
  5. Wait for verdict and return result

Required GitHub Secrets:
    CODECHEF_USERNAME  — Your CodeChef username
    CODECHEF_PASSWORD  — Your CodeChef password
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

_LOGIN_URL: str = "https://www.codechef.com/login"
_BASE_URL: str  = "https://www.codechef.com"
_WAIT_MS: int   = 60_000


# ---------------------------------------------------------------------------
# Browser context
# ---------------------------------------------------------------------------

def _build_context(browser: Browser) -> BrowserContext:
    """Build a browser context, loading saved session if available."""
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


# ---------------------------------------------------------------------------
# Login helpers
# ---------------------------------------------------------------------------

def _is_logged_in(page: Page) -> bool:
    """Return True if the current page shows a logged-in CodeChef user."""
    try:
        # Check auth cookie — most reliable signal
        cookies = page.context.cookies()
        auth_cookie = next(
            (c for c in cookies if c["name"] == "Authorization" and c.get("value")),
            None,
        )
        if auth_cookie:
            logger.debug("CodeChef: auth cookie present — assuming logged in.")
            return True

        # DOM fallback — works on React-rendered pages
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


def _login_with_credentials(page: Page) -> None:
    """
    Log in to CodeChef using CODECHEF_USERNAME and CODECHEF_PASSWORD.
    Raises RuntimeError if login fails.
    """
    logger.info("Logging in to CodeChef as '%s'…", Config.CODECHEF_USERNAME)

    try:
        page.goto(_LOGIN_URL, timeout=_WAIT_MS, wait_until="networkidle")
        time.sleep(2)

        # Already logged in (session carried from storage state)
        if "login" not in page.url or _is_logged_in(page):
            logger.info("CodeChef: already logged in after redirect.")
            return

        # Fill in the login form
        page.locator("input#username").fill(Config.CODECHEF_USERNAME)
        time.sleep(0.5)
        page.locator("input#password").fill(Config.CODECHEF_PASSWORD)
        time.sleep(0.5)

        # Click Login button
        page.locator("button[type='submit']:has-text('Login')").click()

        # Wait until we're redirected away from /login
        try:
            page.wait_for_url(lambda url: "/login" not in url, timeout=20_000)
        except PWTimeout:
            pass

        if "/login" in page.url:
            error_el = page.query_selector(
                ".error-message, [class*='error'], [class*='alert'], [role='alert']"
            )
            error_text = error_el.inner_text().strip() if error_el else "Unknown error"
            raise RuntimeError(
                f"CodeChef login failed — still on login page. "
                f"Error: {error_text}. "
                f"Check CODECHEF_USERNAME and CODECHEF_PASSWORD are correct."
            )

        logger.info("CodeChef login successful. URL: %s", page.url)

    except PWTimeout as exc:
        raise RuntimeError(f"CodeChef login timed out: {exc}") from exc


def _login_with_cookies(page: Page) -> None:
    """
    Inject cookie-based auth (CODECHEF_AUTH_TOKEN etc.) as a fallback.
    Rebuilds the session file from .env values first.
    """
    logger.info("Using cookie-based auth fallback for CodeChef.")
    _build_session_from_env()

    if not _STORAGE_STATE_PATH.exists():
        raise RuntimeError(
            "Cookie-based CodeChef auth failed — session file not created. "
            "Set CODECHEF_AUTH_TOKEN in your GitHub Secrets."
        )

    # Reload the storage state into the current context
    page.context.add_cookies(
        __import__("json").loads(_STORAGE_STATE_PATH.read_text())
        .get("cookies", [])
    )
    page.goto(_BASE_URL, timeout=_WAIT_MS, wait_until="domcontentloaded")
    time.sleep(2)

    if not _is_logged_in(page):
        raise RuntimeError(
            "CodeChef cookie auth failed — cookies are expired. "
            "Update CODECHEF_AUTH_TOKEN / CODECHEF_SESSION in GitHub Secrets."
        )


def _ensure_logged_in(page: Page) -> None:
    """
    Ensure the Playwright session is authenticated.
    Tries credentials first; falls back to cookies.
    """
    if Config.CODECHEF_USERNAME and Config.CODECHEF_PASSWORD:
        _login_with_credentials(page)
    else:
        logger.warning(
            "CODECHEF_USERNAME/PASSWORD not set — trying cookie fallback."
        )
        page.goto(_BASE_URL, timeout=_WAIT_MS, wait_until="domcontentloaded")
        time.sleep(2)
        if not _is_logged_in(page):
            _login_with_cookies(page)


def _save_session(page: Page) -> None:
    """Persist current session cookies to disk for reuse."""
    try:
        _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
        page.context.storage_state(path=str(_STORAGE_STATE_PATH))
        logger.debug("CodeChef session saved to %s", _STORAGE_STATE_PATH)
    except Exception as e:
        logger.warning("Could not save CodeChef session: %s", e)


# ---------------------------------------------------------------------------
# Main submit function
# ---------------------------------------------------------------------------

def submit_solution(problem: dict, code: str) -> dict:
    """
    Submit a solution to CodeChef via Playwright.

    Authenticates using CODECHEF_USERNAME + CODECHEF_PASSWORD (primary), or
    cookie-based auth as a fallback.

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
            # ── 1. Authenticate ─────────────────────────────────────────────
            _ensure_logged_in(page)
            logger.info("CodeChef: authenticated — proceeding to submit.")

            # ── 2. Navigate to the problem page ────────────────────────────
            logger.info("Navigating to problem: %s", url)
            page.goto(url, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(3)

            # ── 3. Click "Solve" or "Submit" button to open editor ─────────
            try:
                page.wait_for_selector(
                    "button:has-text('Submit'), button:has-text('Solve')",
                    timeout=15_000
                )
                solve_btn = page.locator("button:has-text('Solve')").first
                if solve_btn.count() > 0:
                    solve_btn.click()
                    time.sleep(2)
                    logger.info("Clicked 'Solve' to open editor.")
            except PWTimeout:
                logger.warning("No Solve/Submit button found — trying direct submission.")

            # ── 4. Inject code into Monaco editor ─────────────────────────
            time.sleep(2)  # let Monaco initialise
            injected = page.evaluate("""(code) => {
                if (typeof monaco !== 'undefined' && monaco.editor.getModels().length > 0) {
                    monaco.editor.getModels()[0].setValue(code);
                    return true;
                }
                return false;
            }""", code)

            if not injected:
                logger.warning("Monaco not found — trying inputarea clipboard fallback.")
                textarea = page.locator(".inputarea").first
                if textarea.count() > 0:
                    textarea.click()
                    page.keyboard.press("Control+A")
                    page.keyboard.press("Backspace")
                    page.evaluate(
                        "async (text) => await navigator.clipboard.writeText(text)", code
                    )
                    page.keyboard.press("Control+V")
                    time.sleep(1)
                    logger.info("Code injected via clipboard fallback.")
                else:
                    logger.warning("Could not find CodeChef editor — submission may be empty.")

            time.sleep(1)

            # ── 5. Click Submit ────────────────────────────────────────────
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
                verdict  = status_locator.first.inner_text().strip()
                accepted = "Correct Answer" in verdict
                logger.info("CodeChef verdict: %s", verdict)
            except PWTimeout:
                logger.warning("Timed out waiting for CodeChef verdict.")
                verdict = "Unknown Verdict (Timed Out)"

            _save_session(page)

            return {
                "verdict":  verdict,
                "accepted": accepted,
                "url":      page.url,
            }

        finally:
            context.close()
            browser.close()
