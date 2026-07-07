"""
platforms/hackerrank.py — HackerRank daily problem fetcher using Playwright.

HackerRank does not expose a public problem-submission API.  This module uses
a Playwright headless Chromium browser to authenticate and scrape the
recommended or featured daily challenge from the HackerRank dashboard.

Credentials required in .env:
    HACKERRANK_USERNAME — your HackerRank username or email
    HACKERRANK_PASSWORD — your HackerRank account password

Usage:
    from platforms.hackerrank import fetch_daily_problem
    problem = fetch_daily_problem()
    # problem → { title, url, difficulty, description, platform }
"""

import logging
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

# Persistent session storage path
_STORAGE_STATE_PATH: Path = (
    Path(__file__).parent.parent / "logs" / "hackerrank_session.json"
)

_LOGIN_URL: str = "https://www.hackerrank.com/auth/login"
_DASHBOARD_URL: str = "https://www.hackerrank.com/domains/algorithms"
_BASE_URL: str = "https://www.hackerrank.com"

# Playwright timeouts in milliseconds
_WAIT_MS: int = 25_000
_NAV_WAIT_MS: int = 30_000


# ---------------------------------------------------------------------------
# Internal helpers
# ---------------------------------------------------------------------------

def _build_context(browser: Browser) -> BrowserContext:
    """
    Create a Playwright BrowserContext, reusing a previously saved session
    state if one exists on disk.

    Args:
        browser (Browser): An open Playwright Browser instance.

    Returns:
        BrowserContext: A context that may carry pre-loaded cookies from a
                        prior successful login.
    """
    user_agent = (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/124.0.0.0 Safari/537.36"
    )

    if _STORAGE_STATE_PATH.exists():
        logger.debug("Loading existing HackerRank session from %s", _STORAGE_STATE_PATH)
        return browser.new_context(
            storage_state=str(_STORAGE_STATE_PATH),
            user_agent=user_agent,
        )

    logger.debug("No saved HackerRank session — starting fresh context.")
    return browser.new_context(user_agent=user_agent)


def _save_storage_state(context: BrowserContext) -> None:
    """
    Save the current browser context state (cookies, localStorage) to disk.

    Args:
        context (BrowserContext): The Playwright browser context to snapshot.
    """
    _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
    context.storage_state(path=str(_STORAGE_STATE_PATH))
    logger.debug("HackerRank session state saved to %s", _STORAGE_STATE_PATH)


def _is_logged_in(page: Page) -> bool:
    """
    Check whether the current browser session is authenticated on HackerRank.

    Navigates to the dashboard and looks for a DOM element that only appears
    for authenticated users (e.g. a user-menu avatar or username display).

    Args:
        page (Page): The Playwright page to check.

    Returns:
        bool: True if a logged-in indicator element is found; False otherwise.
    """
    try:
        page.goto(_DASHBOARD_URL, timeout=_NAV_WAIT_MS, wait_until="domcontentloaded")
        # If we are redirected to the login page, we are not logged in.
        return "/auth/login" not in page.url
    except Exception:
        return False


def login(page: Page) -> None:
    """
    Authenticate with HackerRank using credentials from Config.

    Navigates to the login page, fills in the username/email and password
    fields, submits the form, and verifies the landing page confirms a
    successful login.

    Args:
        page (Page): A Playwright Page on which the login will be performed.

    Raises:
        RuntimeError: If the login page elements cannot be found, the form
                      submission fails, or the post-login URL still contains
                      '/auth/login' (indicating rejected credentials).

        RuntimeError: With the exact error message text shown on screen if
                      HackerRank renders a visible error element.
    """
    logger.info("Logging in to HackerRank as '%s'…", Config.HACKERRANK_USERNAME)

    try:
        page.goto(_LOGIN_URL, timeout=_NAV_WAIT_MS, wait_until="domcontentloaded")

        # Wait for the login form to be ready
        page.wait_for_selector("input[name='username']", timeout=_WAIT_MS)

        # Fill credentials
        page.fill("input[name='username']", Config.HACKERRANK_USERNAME)
        page.fill("input[name='password']", Config.HACKERRANK_PASSWORD)

        # Submit the login form
        page.click("button[data-analytics='LoginPassword'], button[type='submit']")

        # Wait for SPA navigation away from login
        try:
            page.wait_for_url(lambda url: "auth/login" not in url and "login" not in url.lower(), timeout=15_000)
        except PWTimeout:
            pass # We'll check the current URL and errors below

        current_url: str = page.url

        # If still on login page, surface the on-screen error message
        if "auth/login" in current_url or "login" in current_url.lower():
            error_el = page.query_selector(
                "[class*='error'], .error-message, [data-analytics*='error'], "
                ".flash-error, .notification-error"
            )
            error_text: str = (
                error_el.inner_text().strip()
                if error_el
                else "Credentials rejected — check HACKERRANK_USERNAME and HACKERRANK_PASSWORD."
            )
            raise RuntimeError(
                f"HackerRank login failed. Server message: '{error_text}'. "
                f"Post-submit URL: {current_url}"
            )

        logger.info(
            "HackerRank login successful. Landed on: %s", current_url
        )

    except PWTimeout as exc:
        raise RuntimeError(
            f"HackerRank login timed out waiting for page elements: {exc}"
        ) from exc


def _extract_problem_details(page: Page, problem_url: str) -> dict:
    """
    Navigate to a HackerRank challenge page and extract the title, difficulty,
    and full problem statement.

    Args:
        page        (Page): An authenticated Playwright Page.
        problem_url (str) : Absolute URL to the HackerRank challenge.

    Returns:
        dict: Problem metadata with keys: title, url, difficulty, description,
              platform.

    Raises:
        RuntimeError: If the problem page fails to load or required elements
                      are missing.
    """
    logger.debug("Navigating to HackerRank challenge: %s", problem_url)

    try:
        page.goto(problem_url, timeout=_NAV_WAIT_MS, wait_until="domcontentloaded")

        # Title
        title_el = (
            page.query_selector(".challenge-headline h1")
            or page.query_selector(".challenge-name")
            or page.query_selector("h1")
        )
        title: str = title_el.inner_text().strip() if title_el else "Unknown Challenge"

        # Difficulty badge
        diff_el = (
            page.query_selector(".difficulty-block .difficulty")
            or page.query_selector("[class*='difficulty']")
            or page.query_selector(".challenge-difficulty")
        )
        difficulty: str = diff_el.inner_text().strip() if diff_el else "Unknown"

        # Problem statement body — HackerRank uses a React-rendered div
        # that may need a short wait after initial DOM-ready.
        try:
            page.wait_for_selector(
                ".challenge-body-html, .msB, .problem-statement",
                timeout=_WAIT_MS,
            )
        except PWTimeout:
            logger.warning("Problem body selector timed out for %s", problem_url)

        statement_el = (
            page.query_selector(".challenge-body-html")
            or page.query_selector(".problem-statement")
            or page.query_selector(".msB")
            or page.query_selector("main")
        )
        description: str = (
            statement_el.inner_text().strip() if statement_el
            else f"Problem statement unavailable. Visit: {problem_url}"
        )

        return {
            "title": title,
            "url": problem_url,
            "difficulty": difficulty,
            "description": description,
            "platform": "hackerrank",
        }

    except PWTimeout as exc:
        raise RuntimeError(
            f"HackerRank problem page timed out for {problem_url}: {exc}"
        ) from exc


# ---------------------------------------------------------------------------
# Public API
# ---------------------------------------------------------------------------

def fetch_daily_problem() -> dict:
    """
    Launch a headless Chromium browser, authenticate with HackerRank (or
    reuse a saved session), navigate to the dashboard, and extract the
    recommended or featured daily challenge.

    The function looks for:
    1. A "Recommended Challenge" widget on the dashboard.
    2. A "Daily Challenge" or "Featured" section link.
    3. Falls back to the first challenge link found on the dashboard.

    Returns:
        dict: A problem dictionary with the following keys:
            - title       (str)  : Challenge title
            - url         (str)  : Direct URL to the challenge
            - difficulty  (str)  : Difficulty label (Easy / Medium / Hard)
            - description (str)  : Full plain-text problem statement
            - platform    (str)  : Always "hackerrank"

    Raises:
        RuntimeError: If login fails, the dashboard cannot be loaded, or
                      no challenge link can be located.
    """
    logger.info("Fetching HackerRank daily problem…")

    with sync_playwright() as pw:
        browser = pw.chromium.launch(
            headless=True,
            args=["--disable-blink-features=AutomationControlled"]
        )
        context = _build_context(browser)
        page: Page = context.new_page()

        try:
            # Attempt session reuse; fall back to full login
            if not _is_logged_in(page):
                login(page)
                _save_storage_state(context)

            logger.info("Navigating to HackerRank dashboard…")
            page.goto(_DASHBOARD_URL, timeout=_NAV_WAIT_MS, wait_until="domcontentloaded")

            problem_url: Optional[str] = None

            # --- Strategy 1: Recommended Challenge widget ---
            recommended_el = page.query_selector(
                "[class*='recommended'] a, [data-test='recommended-challenge'] a"
            )
            if recommended_el:
                href = recommended_el.get_attribute("href") or ""
                problem_url = href if href.startswith("http") else f"{_BASE_URL}{href}"
                logger.debug("Found recommended challenge link: %s", problem_url)

            # --- Strategy 2: Daily challenge section ---
            if not problem_url:
                daily_el = page.query_selector(
                    "[class*='daily'] a[href*='/challenges/'], "
                    "[class*='featured'] a[href*='/challenges/']"
                )
                if daily_el:
                    href = daily_el.get_attribute("href") or ""
                    problem_url = href if href.startswith("http") else f"{_BASE_URL}{href}"
                    logger.debug("Found daily challenge link: %s", problem_url)

            # --- Strategy 3: First challenge link on dashboard ---
            if not problem_url:
                logger.warning(
                    "Dedicated daily section not found — using first challenge link."
                )
                first_el = page.query_selector("a[href*='/challenges/']")
                if first_el:
                    href = first_el.get_attribute("href") or ""
                    problem_url = href if href.startswith("http") else f"{_BASE_URL}{href}"
                    logger.debug("Fallback challenge link: %s", problem_url)

            if not problem_url:
                raise RuntimeError(
                    "Could not locate any challenge link on the HackerRank dashboard. "
                    "The page layout may have changed — check logs/hackerrank_session.json."
                )

            problem = _extract_problem_details(page, problem_url)

        except PWTimeout as exc:
            raise RuntimeError(
                f"HackerRank dashboard timed out: {exc}"
            ) from exc
        finally:
            _save_storage_state(context)
            browser.close()

    logger.info(
        "HackerRank daily problem fetched — '%s' (%s)",
        problem["title"],
        problem["difficulty"],
    )
    return problem
