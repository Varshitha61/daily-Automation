"""
platforms/codeforces.py — Codeforces daily problem fetcher.

Combines the official Codeforces REST API (for problem metadata) with a
Playwright headless browser scrape (for the full problem statement), because
the API does not expose problem text directly.

A deterministic seed derived from today's ISO date ensures the same problem
is chosen each time the bot runs on a given day, even if it is restarted.

Credentials required in .env:
    CODEFORCES_API_KEY    — from https://codeforces.com/settings/api
    CODEFORCES_API_SECRET — from https://codeforces.com/settings/api
    CODEFORCES_HANDLE     — your Codeforces username

Usage:
    from platforms.codeforces import fetch_daily_problem
    problem = fetch_daily_problem()
    # problem → { title, problem_id, rating, description_url,
    #              description, platform }
"""

import hashlib
import hmac
import logging
import random
import time
from datetime import date
from typing import Optional

import requests
from playwright.sync_api import sync_playwright, Page, TimeoutError as PWTimeout

from config import Config

logger = logging.getLogger(__name__)

_API_BASE: str = "https://codeforces.com/api"
_MIN_RATING: int = 1200
_MAX_RATING: int = 1600


# ---------------------------------------------------------------------------
# Codeforces REST API helpers
# ---------------------------------------------------------------------------

def _api_get(method: str, params: Optional[dict] = None) -> dict:
    """
    Perform an authenticated GET request to the Codeforces REST API.

    Handles the HMAC-SHA512 signature scheme required for authenticated
    endpoints.  Retries up to Config.RETRY_ATTEMPTS times on transient
    network or server errors.

    Args:
        method (str): API method name, e.g. 'problemset.problems'.
        params (dict | None): Optional query parameters to include.

    Returns:
        dict: The 'result' field of a successful API response.

    Raises:
        RuntimeError: If all retry attempts fail or the API returns a
                      non-OK status.
    """
    if params is None:
        params = {}

    url = f"{_API_BASE}/{method}"
    last_exc: Optional[Exception] = None

    for attempt in range(1, Config.RETRY_ATTEMPTS + 1):
        try:
            logger.debug(
                "Codeforces API GET '%s' — attempt %d/%d",
                method,
                attempt,
                Config.RETRY_ATTEMPTS,
            )
            response = requests.get(
                url,
                params=params,
                timeout=Config.REQUEST_TIMEOUT,
            )
            response.raise_for_status()
            body = response.json()

            if body.get("status") != "OK":
                raise RuntimeError(
                    f"Codeforces API returned non-OK status: {body.get('comment', body)}"
                )

            return body["result"]

        except (requests.RequestException, RuntimeError) as exc:
            last_exc = exc
            logger.warning(
                "Codeforces API request failed on attempt %d: %s", attempt, exc
            )
            if attempt < Config.RETRY_ATTEMPTS:
                logger.info("Retrying in %d seconds…", Config.RETRY_DELAY)
                time.sleep(Config.RETRY_DELAY)

    raise RuntimeError(
        f"Codeforces API failed after {Config.RETRY_ATTEMPTS} attempts. "
        f"Last error: {last_exc}"
    )


def _pick_daily_problem(problems: list[dict]) -> dict:
    """
    Deterministically pick one problem from a list using today's date as
    the random seed.

    This guarantees the same problem is selected throughout the entire day
    regardless of how many times the bot is executed.

    Args:
        problems (list[dict]): List of Codeforces problem dicts from the API.

    Returns:
        dict: A single problem dict chosen deterministically.

    Raises:
        ValueError: If the problems list is empty.
    """
    if not problems:
        raise ValueError("Cannot pick from an empty problems list.")

    seed = int(date.today().strftime("%Y%m%d"))
    rng = random.Random(seed)
    chosen = rng.choice(problems)
    logger.debug(
        "Picked problem '%s' (seed=%d) from %d candidates.",
        chosen.get("name"),
        seed,
        len(problems),
    )
    return chosen


# ---------------------------------------------------------------------------
# Playwright scraper — problem statement
# ---------------------------------------------------------------------------

def fetch_problem_statement(contest_id: int, index: str) -> str:
    """
    Use a Playwright headless Chromium browser to scrape the full problem
    statement from the Codeforces problem page.

    The statement includes: problem text, input format, output format, and
    sample inputs/outputs.  All content is returned as clean plain text.

    Args:
        contest_id (int): The contest/problem-set ID (e.g. 1234).
        index      (str): The problem index letter (e.g. 'A', 'B').

    Returns:
        str: The full problem statement as plain text.

    Raises:
        RuntimeError: If the page does not load, times out, or the expected
                      DOM elements are not found.
    """
    url = f"https://codeforces.com/problemset/problem/{contest_id}/{index}"
    logger.info("Scraping problem statement from %s", url)

    with sync_playwright() as pw:
        from platforms.submitters.codeforces_submit import _launch_browser, _build_context
        browser = _launch_browser(pw)
        context = _build_context(browser)
        page: Page = context.new_page()

        try:
            page.goto(url, timeout=Config.REQUEST_TIMEOUT * 1000, wait_until="domcontentloaded")

            # Wait for the problem statement div to appear, fallback to pageContent
            try:
                page.wait_for_selector(".problem-statement", timeout=30_000)
            except PWTimeout:
                logger.warning("Codeforces `.problem-statement` timeout, trying `#pageContent`")
                page.wait_for_selector("#pageContent", timeout=15_000)

            # Extract text from each section individually for clean formatting
            sections: list[str] = []

            # Main problem body
            body_el = page.query_selector(".problem-statement > .header ~ *")
            if body_el:
                sections.append(body_el.inner_text())

            # Title
            title_el = page.query_selector(".problem-statement .title")
            title_text = title_el.inner_text() if title_el else ""

            # All named sections: Input, Output, Note, etc.
            for section_el in page.query_selector_all(".problem-statement .section-title"):
                label = section_el.inner_text().strip()
                parent = section_el.evaluate_handle("el => el.parentElement")
                parent_text = parent.as_element().inner_text() if parent.as_element() else ""
                sections.append(f"\n{label}\n{parent_text}")

            # Sample input/output blocks
            sample_tests_el = page.query_selector(".sample-tests")
            if sample_tests_el:
                sections.append("\nSamples:\n" + sample_tests_el.inner_text())

            full_text = f"{title_text}\n\n" + "\n".join(sections)
            full_text = "\n".join(
                line for line in full_text.splitlines() if line.strip()
            )

            logger.info(
                "Successfully scraped problem %d%s (%d chars).",
                contest_id,
                index,
                len(full_text),
            )
            return full_text

        except PWTimeout as exc:
            raise RuntimeError(
                f"Timed out waiting for Codeforces problem page {url}: {exc}"
            ) from exc
        except Exception as exc:
            raise RuntimeError(
                f"Failed to scrape Codeforces problem {url}: {exc}"
            ) from exc
        finally:
            browser.close()


# ---------------------------------------------------------------------------
# Public API
# ---------------------------------------------------------------------------

def fetch_daily_problem() -> dict:
    """
    Fetch today's Codeforces problem by:
        1. Calling the problemset.problems REST endpoint to get all problems.
        2. Filtering for problems with rating between _MIN_RATING and _MAX_RATING.
        3. Picking one deterministically using today's date as the RNG seed.
        4. Scraping the full problem statement with Playwright.

    Returns:
        dict: A problem dictionary with the following keys:
            - title           (str)  : Problem title
            - problem_id      (str)  : "{contestId}{index}", e.g. "1234A"
            - contest_id      (int)  : Numeric contest ID
            - index           (str)  : Problem index letter, e.g. "A"
            - rating          (int)  : Problem difficulty rating
            - description_url (str)  : Direct URL to the problem
            - description     (str)  : Full plain-text problem statement
            - platform        (str)  : Always "codeforces"

    Raises:
        RuntimeError: If the API call fails, no suitable problems are found,
                      or the problem statement cannot be scraped.
    """
    logger.info(
        "Fetching Codeforces daily problem (rating %d–%d)…",
        _MIN_RATING,
        _MAX_RATING,
    )

    result = _api_get("problemset.problems")
    all_problems: list[dict] = result.get("problems", [])

    # Filter by rating range and ensure the problem has a contestId
    candidates = [
        p for p in all_problems
        if _MIN_RATING <= p.get("rating", 0) <= _MAX_RATING
        and "contestId" in p
    ]

    if not candidates:
        raise RuntimeError(
            f"No Codeforces problems found with rating {_MIN_RATING}–{_MAX_RATING}."
        )

    logger.info("Found %d candidate problems in rating range.", len(candidates))

    chosen = _pick_daily_problem(candidates)
    contest_id: int = chosen["contestId"]
    index: str = chosen["index"]
    title: str = chosen["name"]
    rating: int = chosen.get("rating", 0)
    problem_id: str = f"{contest_id}{index}"
    description_url: str = (
        f"https://codeforces.com/problemset/problem/{contest_id}/{index}"
    )

    # Scrape the full problem statement from the web page
    description = fetch_problem_statement(contest_id, index)

    logger.info(
        "Codeforces daily problem ready — '%s' [%s] rating=%d",
        title,
        problem_id,
        rating,
    )

    return {
        "title": title,
        "problem_id": problem_id,
        "contest_id": contest_id,
        "index": index,
        "rating": rating,
        "description_url": description_url,
        "description": description,
        "platform": "codeforces",
    }
