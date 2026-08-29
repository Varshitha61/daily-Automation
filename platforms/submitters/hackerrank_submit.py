"""
platforms/submitters/hackerrank_submit.py — HackerRank solution submitter using Playwright.

Navigates to the problem page, logs in if necessary (using credentials from
Config), injects the generated code into the editor, and submits.

GitHub Actions compatibility:
  - No session file is available between runs in CI, so this submitter
    performs a full login using HACKERRANK_USERNAME and HACKERRANK_PASSWORD
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

logger = logging.getLogger(__name__)

_STORAGE_STATE_PATH: Path = (
    Path(__file__).parent.parent.parent / "logs" / "hackerrank_session.json"
)

_LOGIN_URL: str = "https://www.hackerrank.com/auth/login"
_BASE_URL: str = "https://www.hackerrank.com"
_WAIT_MS: int = 45_000


def _build_context(browser: Browser) -> BrowserContext:
    user_agent = (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/124.0.0.0 Safari/537.36"
    )

    if _STORAGE_STATE_PATH.exists():
        logger.info("Loading saved HackerRank session from %s", _STORAGE_STATE_PATH)
        return browser.new_context(
            storage_state=str(_STORAGE_STATE_PATH),
            user_agent=user_agent,
            permissions=["clipboard-read", "clipboard-write"],
        )

    logger.info("No saved HackerRank session — starting fresh context.")
    return browser.new_context(
        user_agent=user_agent,
        permissions=["clipboard-read", "clipboard-write"],
    )


def _is_logged_in(page: Page) -> bool:
    """Return True if the page is NOT on the login screen."""
    return "/auth/login" not in page.url


def _login(page: Page) -> None:
    """
    Log in to HackerRank using credentials from Config.
    Raises RuntimeError if login fails.
    """
    logger.info("Logging in to HackerRank as '%s'…", Config.HACKERRANK_USERNAME)

    try:
        page.goto(_LOGIN_URL, timeout=_WAIT_MS, wait_until="domcontentloaded")

        # Wait for form
        page.wait_for_selector("input[name='username']", timeout=_WAIT_MS)

        page.fill("input[name='username']", Config.HACKERRANK_USERNAME)
        page.fill("input[name='password']", Config.HACKERRANK_PASSWORD)

        page.click("button[data-analytics='LoginPassword'], button[type='submit']")

        try:
            page.wait_for_url(
                lambda url: "auth/login" not in url and "login" not in url.lower(),
                timeout=15_000
            )
        except PWTimeout:
            pass

        if "auth/login" in page.url or "login" in page.url.lower():
            error_el = page.query_selector(
                "[class*='error'], .error-message, .flash-error"
            )
            error_text = (
                error_el.inner_text().strip()
                if error_el
                else "Credentials rejected — check HACKERRANK_USERNAME and HACKERRANK_PASSWORD."
            )
            raise RuntimeError(
                f"HackerRank login failed: '{error_text}'. URL: {page.url}"
            )

        logger.info("HackerRank login successful. URL: %s", page.url)

    except PWTimeout as exc:
        raise RuntimeError(f"HackerRank login timed out: {exc}") from exc


def _save_session(page: Page) -> None:
    """Persist current session cookies to disk for reuse."""
    try:
        _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
        page.context.storage_state(path=str(_STORAGE_STATE_PATH))
        logger.debug("HackerRank session saved to %s", _STORAGE_STATE_PATH)
    except Exception as e:
        logger.warning("Could not save HackerRank session: %s", e)


def submit_solution(problem: dict, code: str) -> dict:
    """
    Submit a solution to HackerRank via Playwright.

    Logs in using HACKERRANK_USERNAME/HACKERRANK_PASSWORD if no saved session
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
        raise RuntimeError("HackerRank submitter: missing 'url' in problem dict.")

    logger.info("Submitting HackerRank solution to %s", url)

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
            # ── 1. Check authentication ────────────────────────────────────
            # Navigate to base URL to check if session is valid
            page.goto(_BASE_URL, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(2)

            # If redirected to login, perform a full login
            if not _is_logged_in(page):
                _login(page)
                _save_session(page)
            else:
                logger.info("HackerRank: session is valid.")

            # ── 2. Navigate to the problem page ────────────────────────────
            page.goto(url, timeout=_WAIT_MS, wait_until="domcontentloaded")
            time.sleep(3)

            # ── 3. Wait for the code editor to load ────────────────────────
            try:
                page.wait_for_selector(
                    ".view-lines, .inputarea, .CodeMirror",
                    timeout=20_000
                )
            except PWTimeout:
                logger.warning("Could not find HackerRank editor on the page.")

            # ── 4. Inject code into editor ─────────────────────────────────
            injected = page.evaluate("""(code) => {
                if (typeof monaco !== 'undefined' && monaco.editor.getModels().length > 0) {
                    monaco.editor.getModels()[0].setValue(code);
                    return true;
                }
                let cm = document.querySelector('.CodeMirror');
                if (cm && cm.CodeMirror) {
                    cm.CodeMirror.setValue(code);
                    return true;
                }
                return false;
            }""", code)

            if not injected:
                logger.warning("Could not find HackerRank editor via JS. Trying fallback.")
                textarea = page.locator(".inputarea").first
                if textarea.count() > 0:
                    textarea.click()
                    page.keyboard.press("Control+A")
                    page.keyboard.press("Backspace")
                    page.evaluate("async (text) => await navigator.clipboard.writeText(text)", code)
                    page.keyboard.press("Control+V")
                    time.sleep(1)
                else:
                    logger.warning("Could not find HackerRank editor textarea fallback.")

            # ── 5. Click Submit Code ───────────────────────────────────────
            try:
                submit_btn = page.locator("button:has-text('Submit Code'), button:has-text('Submit')")
                submit_btn.first.click(force=True)
                logger.info("Clicked HackerRank Submit button.")
            except Exception as e:
                logger.warning("Failed to click Submit: %s", e)

            # ── 6. Wait for verdict ────────────────────────────────────────
            verdict = "Submitted"
            accepted = False

            try:
                # Expanded selectors based on known HackerRank classes
                status_locator = page.locator(
                    ".test-case-status, .compiler-message, .status-title, "
                    "[class*='verdict'], [class*='result-txt'], "
                    ".success-msg, .error-msg, .compile-error, "
                    ".hr-dialog-content, .status"
                )
                status_locator.first.wait_for(timeout=45_000)
                verdict = status_locator.first.inner_text().strip()
                accepted = (
                    "Congratulations" in verdict
                    or "Accepted" in verdict
                    or "Success" in verdict
                )
                logger.info("HackerRank verdict: %s", verdict)
            except PWTimeout:
                logger.warning("Timed out waiting for verdict on HackerRank.")
                page.screenshot(path="logs/hackerrank_timeout.png")
                logger.info("Saved timeout screenshot to logs/hackerrank_timeout.png")
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
