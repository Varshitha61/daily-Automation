"""
platforms/codechef.py — CodeChef daily problem fetcher using Playwright.

CodeChef has no public API, so this module uses a headless Chromium browser
(via Playwright) to navigate the practice section. Authentication is done
via browser cookies stored in .env — no login form needed.

Credentials required in .env:
    CODECHEF_AUTH_TOKEN   — value of the 'Authorization' cookie (JWT)
    CODECHEF_SESSION      — value of the 'SESS93b6022d...' session cookie
    CODECHEF_CF_CLEARANCE — value of the 'cf_clearance' Cloudflare cookie
    CODECHEF_UID          — value of the 'uid' cookie
    CODECHEF_USERKEY      — value of the 'userkey' cookie

Usage:
    from platforms.codechef import fetch_daily_problem
    problem = fetch_daily_problem()
    # problem → { title, url, difficulty, description, platform }
"""

import json
import logging
import random
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

def _build_session_from_env() -> None:
    """
    Build the Playwright storage state JSON from CodeChef cookie values
    stored in .env / Config. Called automatically before each run so the
    session file is always up-to-date with the latest .env values.
    """
    if not Config.CODECHEF_AUTH_TOKEN:
        logger.warning("CODECHEF_AUTH_TOKEN not set — CodeChef may not authenticate.")
        return

    cookies = [
        {
            "name": "Authorization",
            "value": Config.CODECHEF_AUTH_TOKEN,
            "domain": "www.codechef.com", "path": "/",
            "expires": 1758226976.0,
            "httpOnly": True, "secure": True, "sameSite": "Lax",
        },
        {
            "name": "SESS93b6022d778ee317bf48f7dbffe03173",
            "value": Config.CODECHEF_SESSION,
            "domain": ".codechef.com", "path": "/",
            "expires": 1758226976.0,
            "httpOnly": True, "secure": True, "sameSite": "Lax",
        },
        {
            "name": "cf_clearance",
            "value": Config.CODECHEF_CF_CLEARANCE,
            "domain": ".codechef.com", "path": "/",
            "expires": 1817121494.0,
            "httpOnly": True, "secure": True, "sameSite": "None",
        },
        {
            "name": "uid",
            "value": Config.CODECHEF_UID,
            "domain": "www.codechef.com", "path": "/",
            "expires": 1758226976.0,
            "httpOnly": False, "secure": False, "sameSite": "Lax",
        },
        {
            "name": "userkey",
            "value": Config.CODECHEF_USERKEY,
            "domain": "www.codechef.com", "path": "/",
            "expires": 1756518000.0,
            "httpOnly": False, "secure": False, "sameSite": "Lax",
        },
    ]
    # Filter out empty cookies
    cookies = [c for c in cookies if c["value"]]

    state = {"cookies": cookies, "origins": []}
    _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
    _STORAGE_STATE_PATH.write_text(json.dumps(state, indent=2))
    logger.info("CodeChef session built from .env (%d cookies)", len(cookies))


def _build_context(browser: Browser) -> BrowserContext:
    """
    Create a Playwright BrowserContext loaded with cookies from the
    session file (which is auto-built from .env values).

    Args:
        browser (Browser): An open Playwright Browser instance.

    Returns:
        BrowserContext: A context with cookies pre-loaded.
    """
    user_agent = (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/124.0.0.0 Safari/537.36"
    )

    if _STORAGE_STATE_PATH.exists():
        logger.debug("Loading CodeChef session from %s", _STORAGE_STATE_PATH)
        return browser.new_context(
            storage_state=str(_STORAGE_STATE_PATH),
            user_agent=user_agent,
        )

    logger.debug("No CodeChef session file found — using fresh context.")
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
    page.wait_for_timeout(4000)  # wait for React to render

    # Title — try multiple selectors for different CodeChef layouts
    title_el = (
        page.query_selector("h1.title-txt")
        or page.query_selector("[class*='problem-title']")
        or page.query_selector("[class*='ProblemTitle']")
        or page.query_selector("h1[class*='problem']")
        or page.query_selector(".problem-name h1")
        or page.query_selector("h1")
    )
    title: str = title_el.inner_text().strip() if title_el else "Unknown Problem"

    # Difficulty badge
    diff_el = page.query_selector("[class*='difficulty'], [data-difficulty], .difficulty-rating")
    difficulty: str = diff_el.inner_text().strip() if diff_el else "Unknown"

    # Problem statement — try multiple selectors for different layouts
    statement_el = (
        page.query_selector(".problem-statement")
        or page.query_selector("#problem-statement")
        or page.query_selector(".statement-body")
        or page.query_selector("[class*='ProblemStatement']")
        or page.query_selector("[class*='problem-statement']")
        or page.query_selector("._content__KPo3")
        or page.query_selector("main article")
        or page.query_selector("main")
    )
    description: str = statement_el.inner_text().strip() if statement_el else ""

    # Last resort: grab all visible text from the page body
    if not description or len(description) < 50:
        logger.warning("Standard selectors failed — trying full page text for %s", problem_url)
        try:
            description = page.evaluate("""() => {
                const main = document.querySelector('main') || document.body;
                return main.innerText;
            }""")
            if description:
                description = description.strip()
        except Exception:
            pass

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
    Launch a headless Chromium browser, authenticate via cookies from .env,
    navigate to the Practice page, and fetch the first problem.

    No login form is used — cookies are injected directly from Config.

    Returns:
        dict: A problem dictionary with the following keys:
            - title       (str)  : Problem title
            - url         (str)  : Direct URL to the problem
            - difficulty  (str)  : Difficulty label (e.g. "Easy", "Medium")
            - description (str)  : Full plain-text problem statement
            - platform    (str)  : Always "codechef"

    Raises:
        RuntimeError: If the practice page cannot be navigated or no
                      problems are found.
    """
    logger.info("Fetching CodeChef daily problem…")

    # Always rebuild session file from .env so latest cookies are used
    _build_session_from_env()

    with sync_playwright() as pw:
        browser = pw.chromium.launch(
            headless=True,
            args=[
                "--disable-blink-features=AutomationControlled",
                "--no-sandbox",
                "--disable-setuid-sandbox",
            ]
        )
        context = _build_context(browser)
        page: Page = context.new_page()

        try:
            logger.info("Navigating to CodeChef Practice page…")
            page.goto(_PRACTICE_URL, timeout=_WAIT_MS, wait_until="networkidle")
            page.wait_for_timeout(3000)

            # Look for Daily Practice section problems
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

            # Fallback: grab all problem links visible on the practice page
            if not problem_link:
                logger.warning(
                    "Daily Practice section not found — picking a random "
                    "problem link from the practice page."
                )
                # query_selector_all gets all matching elements
                fallback_els = page.query_selector_all("a[href*='/problems/']")
                
                # Filter out discussion/editorial links, keep only direct problem links
                valid_links = []
                for el in fallback_els:
                    href = el.get_attribute("href")
                    if href and "/problems/" in href and "/viewsolution" not in href and "discuss" not in href:
                        valid_links.append(href)
                
                if valid_links:
                    # Pick a random problem so it's a new one each day
                    href = random.choice(valid_links)
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
            browser.close()

    logger.info(
        "CodeChef daily problem fetched — '%s' (%s)",
        problem["title"],
        problem["difficulty"],
    )
    return problem
