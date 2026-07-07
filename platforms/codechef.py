"""
platforms/codechef.py — CodeChef daily problem fetcher using Playwright.

CodeChef has no public API, so this module uses a headless Chromium browser
(via Playwright) to log in and navigate the practice section.  A persistent
browser context is used so the login session cookie is reused between runs
within the same process.

Credentials required in .env:
    CODECHEF_USERNAME — your CodeChef login username or email
    CODECHEF_PASSWORD — your CodeChef login password

Usage:
    from platforms.codechef import fetch_daily_problem
    problem = fetch_daily_problem()
    # problem → { title, url, difficulty, description, platform }
"""

import logging
import os
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

# Path to store the persistent browser profile so cookies survive restarts.
_STORAGE_STATE_PATH: Path = (
    Path(__file__).parent.parent / "logs" / "codechef_session.json"
)

_LOGIN_URL: str = "https://www.codechef.com/login"
_PRACTICE_URL: str = "https://www.codechef.com/practice/logical-problems"
_BASE_URL: str = "https://www.codechef.com"

# How long (ms) to wait for page elements before raising a timeout.
_WAIT_MS: int = 45_000


# ---------------------------------------------------------------------------
# Internal helpers
# ---------------------------------------------------------------------------

def _save_storage_state(context: BrowserContext) -> None:
    """
    Persist the browser context's cookies and local storage to disk.

    This allows the bot to reuse an existing authenticated session on the
    next run without logging in again, reducing load on CodeChef's login
    endpoint and avoiding rate-limiting.

    Args:
        context (BrowserContext): The Playwright browser context to snapshot.
    """
    _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
    context.storage_state(path=str(_STORAGE_STATE_PATH))
    logger.debug("CodeChef session state saved to %s", _STORAGE_STATE_PATH)


def _build_context(browser: Browser) -> BrowserContext:
    """
    Create a Playwright BrowserContext, loading any previously saved session
    state from disk if it exists.

    Args:
        browser (Browser): An open Playwright Browser instance.

    Returns:
        BrowserContext: A context with cookies pre-loaded from disk (if
                        available) or a fresh context if no saved state exists.
    """
    user_agent = (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/124.0.0.0 Safari/537.36"
    )

    if _STORAGE_STATE_PATH.exists():
        logger.debug("Loading existing CodeChef session from %s", _STORAGE_STATE_PATH)
        return browser.new_context(
            storage_state=str(_STORAGE_STATE_PATH),
            user_agent=user_agent,
        )

    logger.debug("No existing CodeChef session found — creating fresh context.")
    return browser.new_context(user_agent=user_agent)


def _is_logged_in(page: Page) -> bool:
    """
    Check whether the current page state indicates an authenticated session.

    Looks for the presence of an account/profile menu element that only
    appears for logged-in users.

    Args:
        page (Page): The current Playwright page object.

    Returns:
        bool: True if the user appears to be logged in; False otherwise.
    """
    try:
        page.goto(_BASE_URL, timeout=_WAIT_MS, wait_until="domcontentloaded")
        # Check for any element containing 'logout' or 'user-dropdown' in its class name
        el = page.query_selector("[class*='logout'], [class*='username-dropdown']")
        return el is not None
    except Exception:
        return False


def login(page: Page) -> None:
    """
    Log in to CodeChef using credentials from Config.

    Navigates to the login page, fills the username and password fields,
    submits the form, and waits for the navigation to confirm a successful
    login.

    Args:
        page (Page): A Playwright Page object on which the login flow will
                     be performed.

    Raises:
        RuntimeError: If the login form cannot be filled, the submit button
                      is not found, or the page does not navigate away from
                      the login URL (indicating a failed login, e.g. wrong
                      credentials).
    """
    logger.info("Logging in to CodeChef as '%s'…", Config.CODECHEF_USERNAME)

    try:
        page.goto(_LOGIN_URL, timeout=_WAIT_MS, wait_until="networkidle")

        # If CodeChef redirects us away from the login page, we are already logged in
        if "login" not in page.url:
            return

        # Fill username / email targeting the specific login form and bypassing visibility checks
        page.locator("form#ajax-login-form input[name='name']").fill(Config.CODECHEF_USERNAME, force=True)

        # Fill password
        page.locator("form#ajax-login-form input[name='pass']").fill(Config.CODECHEF_PASSWORD, force=True)
        
        # Click the actual login button
        page.locator("input.cc-login-btn").click(force=True)

        # Wait for either the dashboard URL or an error message to appear
        try:
            page.wait_for_url(lambda url: "/login" not in url, timeout=10_000)
        except PWTimeout:
            pass # We'll check the current URL and errors below

        current_url: str = page.url
        if "/login" in current_url:
            # Still on the login page — credentials rejected
            error_el = page.query_selector(".messages--error, .error-message, [class*='error']")
            error_text = error_el.inner_text() if error_el else "Unknown login error"
            raise RuntimeError(
                f"CodeChef login failed — still on login page after submit. "
                f"Error: {error_text}"
            )

        logger.info("CodeChef login successful. Current URL: %s", current_url)

    except PWTimeout as exc:
        raise RuntimeError(
            f"CodeChef login timed out while waiting for page elements: {exc}"
        ) from exc


def _extract_problem_from_page(page: Page, problem_url: str) -> dict:
    """
    Navigate to a specific CodeChef problem page and extract its title,
    difficulty, and full problem statement.

    Args:
        page        (Page): An authenticated Playwright Page.
        problem_url (str) : Absolute URL to the CodeChef problem.

    Returns:
        dict: Problem metadata with keys: title, url, difficulty, description,
              platform.

    Raises:
        RuntimeError: If required DOM elements cannot be found on the problem
                      page.
    """
    logger.debug("Navigating to problem page: %s", problem_url)
    page.goto(problem_url, timeout=_WAIT_MS, wait_until="domcontentloaded")

    # Title — CodeChef uses an h1 or a specific class
    title_el = page.query_selector("h1.title-txt, h1[class*='problem'], .problem-name h1, h1")
    title: str = title_el.inner_text().strip() if title_el else "Unknown Problem"

    # Difficulty badge
    diff_el = page.query_selector("[class*='difficulty'], [data-difficulty], .difficulty-rating")
    difficulty: str = diff_el.inner_text().strip() if diff_el else "Unknown"

    # Problem statement — try multiple selectors for different layouts
    statement_el = (
        page.query_selector(".problem-statement")
        or page.query_selector("#problem-statement")
        or page.query_selector(".statement-body")
        or page.query_selector("main article")
    )
    description: str = statement_el.inner_text().strip() if statement_el else ""

    if not description:
        logger.warning("Could not extract problem description from %s", problem_url)
        description = f"Problem statement unavailable. Visit: {problem_url}"

    return {
        "title": title,
        "url": problem_url,
        "difficulty": difficulty,
        "description": description,
        "platform": "codechef",
    }


# ---------------------------------------------------------------------------
# Public API
# ---------------------------------------------------------------------------

def fetch_daily_problem() -> dict:
    """
    Launch a headless Chromium browser, log in to CodeChef (or reuse a saved
    session), navigate to the Practice page, and fetch the first unsolved
    problem from the Daily Practice section.

    The browser context is persisted between calls within the same process to
    avoid repeated logins.

    Returns:
        dict: A problem dictionary with the following keys:
            - title       (str)  : Problem title
            - url         (str)  : Direct URL to the problem
            - difficulty  (str)  : Difficulty label (e.g. "Easy", "Medium")
            - description (str)  : Full plain-text problem statement
            - platform    (str)  : Always "codechef"

    Raises:
        RuntimeError: If login fails, the practice page cannot be navigated,
                      or no problems are found in the daily section.
    """
    logger.info("Fetching CodeChef daily problem…")

    with sync_playwright() as pw:
        browser = pw.chromium.launch(
            headless=True,
            args=["--disable-blink-features=AutomationControlled"]
        )
        context = _build_context(browser)
        page: Page = context.new_page()

        try:
            # Attempt to reuse existing session first
            if not _is_logged_in(page):
                login(page)
                _save_storage_state(context)

            logger.info("Navigating to CodeChef Practice page…")
            page.goto(_PRACTICE_URL, timeout=_WAIT_MS, wait_until="networkidle")
            page.wait_for_timeout(3000)

            # Look for Daily Practice section problems
            # CodeChef's Practice page lists problems in card-style rows.
            problem_link: Optional[str] = None

            # Strategy: find any link inside a "daily" heading section
            daily_section = (
                page.query_selector("[class*='daily'], [data-section='daily'], #daily-practice")
            )

            if daily_section:
                link_el = daily_section.query_selector("a[href*='/problems/']")
                if link_el:
                    href = link_el.get_attribute("href")
                    problem_link = href if href.startswith("http") else f"{_BASE_URL}{href}"

            # Fallback: grab the first problem link visible on the practice page
            if not problem_link:
                logger.warning(
                    "Daily Practice section not found — falling back to first "
                    "problem link on the practice page."
                )
                fallback_el = page.query_selector("a[href*='/problems/']")
                if fallback_el:
                    href = fallback_el.get_attribute("href") or ""
                    problem_link = href if href.startswith("http") else f"{_BASE_URL}{href}"

            if not problem_link:
                raise RuntimeError(
                    "Could not find any problem link on the CodeChef Practice page. "
                    "The page layout may have changed."
                )

            logger.info("Found problem link: %s", problem_link)
            problem = _extract_problem_from_page(page, problem_link)

        except PWTimeout as exc:
            raise RuntimeError(
                f"CodeChef page timed out: {exc}"
            ) from exc
        finally:
            _save_storage_state(context)
            browser.close()

    logger.info(
        "CodeChef daily problem fetched — '%s' (%s)",
        problem["title"],
        problem["difficulty"],
    )
    return problem
