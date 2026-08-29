"""
platforms/submitters/codechef_submit.py — CodeChef solution submitter using Playwright.

Navigates to the problem page, logs in if necessary (using credentials from
Config), injects the generated code into the Monaco editor, and submits.

GitHub Actions compatibility:
  - No session file is available between runs in CI, so this submitter
    performs a full login using CODECHEF_USERNAME and CODECHEF_PASSWORD
    every time the session file is absent.
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

# Path to the persistent browser profile created by codechef.py fetcher
_STORAGE_STATE_PATH: Path = (
    Path(__file__).parent.parent.parent / "logs" / "codechef_session.json"
)

_LOGIN_URL: str = "https://www.codechef.com/login"
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
    """Return True if the CodeChef page shows a logged-in user element."""
    try:
        el = page.query_selector("[class*='logout'], [class*='username-dropdown'], [class*='user-avatar']")
        return el is not None
    except Exception:
        return False


def _login(page: Page) -> None:
    """
    Log in to CodeChef using credentials from Config.
    Raises RuntimeError if login fails.
    """
    logger.info("Logging in to CodeChef as '%s'…", Config.CODECHEF_USERNAME)

    try:
        page.goto(_LOGIN_URL, timeout=_WAIT_MS, wait_until="networkidle")

        # Already redirected away from login — session is valid
        if "login" not in page.url:
            logger.info("CodeChef: already logged in after redirect.")
            return

        # Fill login form
        page.locator("form#ajax-login-form input[name='name']").fill(
            Config.CODECHEF_USERNAME, force=True
        )
        page.locator("form#ajax-login-form input[name='pass']").fill(
            Config.CODECHEF_PASSWORD, force=True
        )
        page.locator("input.cc-login-btn").click(force=True)

        try:
            page.wait_for_url(lambda url: "/login" not in url, timeout=15_000)
        except PWTimeout:
            pass

        if "/login" in page.url:
            error_el = page.query_selector(".messages--error, .error-message, [class*='error']")
            error_text = error_el.inner_text() if error_el else "Unknown login error"
            raise RuntimeError(
                f"CodeChef login failed — still on login page. Error: {error_text}"
            )

        logger.info("CodeChef login successful. URL: %s", page.url)

    except PWTimeout as exc:
        raise RuntimeError(
            f"CodeChef login timed out: {exc}"
        ) from exc


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

    Logs in using CODECHEF_USERNAME/CODECHEF_PASSWORD if no saved session
    is available (required for GitHub Actions runs).

    Args:
        problem (dict): Must contain 'url' (the problem page URL).
        code    (str): Full source code to submit.

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
            # Navigate directly to home to verify cookies work
            page.goto(_BASE_URL, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(2)

            if not _is_logged_in(page):
                raise RuntimeError("Your CodeChef cookies (CODECHEF_AUTH_TOKEN, etc.) have expired. Please log into codechef.com, copy the new cookies, and update your GitHub Actions Secrets.")

            # ── 2. Navigate to the problem page ────────────────────────────
            page.goto(url, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(3)

            # ── 3. Click Submit/Solve button to enter submit mode ──────────
            try:
                # Try to find and click "Submit" button first
                page.wait_for_selector(
                    "button:has-text('Submit'), button:has-text('Solve')",
                    timeout=15_000
                )
                submit_or_solve = page.locator("button:has-text('Solve')").first
                if submit_or_solve.count() > 0:
                    submit_or_solve.click()
                    time.sleep(2)
            except PWTimeout:
                logger.warning("No Submit/Solve button found on CodeChef problem page.")

            # ── 4. Inject code into Monaco editor ─────────────────────────
            injected = page.evaluate("""(code) => {
                if (typeof monaco !== 'undefined' && monaco.editor.getModels().length > 0) {
                    monaco.editor.getModels()[0].setValue(code);
                    return true;
                }
                return false;
            }""", code)

            if not injected:
                logger.warning("Monaco not found — trying textarea fallback.")
                textarea = page.locator(".inputarea").first
                if textarea.count() > 0:
                    textarea.click()
                    page.keyboard.press("Control+A")
                    page.keyboard.press("Backspace")
                    page.evaluate("async (text) => await navigator.clipboard.writeText(text)", code)
                    page.keyboard.press("Control+V")
                    time.sleep(1)
                else:
                    logger.warning("Could not find CodeChef editor textarea.")

            time.sleep(1)

            # ── 5. Click the Submit button ─────────────────────────────────
            try:
                # Use the last Submit button visible (after code entry)
                submit_btn = page.locator("button:has-text('Submit')").last
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
                    .or_(page.locator("[class*='verdict'], [class*='result']"))
                )
                status_locator.first.wait_for(timeout=40_000)
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
