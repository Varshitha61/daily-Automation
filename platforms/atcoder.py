"""
platforms/atcoder.py — AtCoder daily problem fetcher using Playwright.

Deterministic seed derived from today's ISO date ensures the same problem
is chosen each time the bot runs on a given day.

Usage:
    from platforms.atcoder import fetch_daily_problem
    problem = fetch_daily_problem()
    # problem → { title, url, difficulty, description, platform, contest_id, problem_id, task_char }
"""

import logging
import random
from datetime import date
from playwright.sync_api import sync_playwright, Page, TimeoutError as PWTimeout

from config import Config

logger = logging.getLogger(__name__)

# Choose a random ABC contest between 100 and 350
_MIN_CONTEST: int = 100
_MAX_CONTEST: int = 350


def fetch_daily_problem() -> dict:
    """
    Fetch today's AtCoder problem by:
        1. Deterministically choosing an ABC contest and problem index ('b' or 'c') based on today's date.
        2. Navigating to the task URL on atcoder.jp.
        3. Scraping the English problem statement (.lang-en) using Playwright.

    Returns:
        dict: A problem dictionary with the following keys:
            - title       (str)  : Problem title
            - url         (str)  : Direct URL to the problem
            - difficulty  (str)  : "Easy" | "Medium"
            - description (str)  : Full English problem description
            - platform    (str)  : Always "atcoder"
            - contest_id  (str)  : e.g. "abc300"
            - problem_id  (str)  : e.g. "abc300_b"
            - task_char   (str)  : e.g. "b"
    """
    logger.info("Fetching AtCoder daily problem…")

    # Deterministically choose the contest and problem based on today's date
    seed = int(date.today().strftime("%Y%m%d"))
    rng = random.Random(seed)
    contest_num = rng.randint(_MIN_CONTEST, _MAX_CONTEST)
    task_char = rng.choice(["b", "c"])

    contest_id = f"abc{contest_num}"
    problem_id = f"abc{contest_num}_{task_char}"
    problem_url = f"https://atcoder.jp/contests/{contest_id}/tasks/{problem_id}"

    logger.info(
        "Deterministic choice: Contest=%s, Task=%s, URL=%s",
        contest_id,
        problem_id,
        problem_url,
    )

    with sync_playwright() as pw:
        browser = pw.chromium.launch(
            headless=True,
            args=["--disable-blink-features=AutomationControlled"]
        )
        context = browser.new_context(
            user_agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36"
        )
        page: Page = context.new_page()

        try:
            page.goto(
                problem_url,
                timeout=Config.REQUEST_TIMEOUT * 1000,
                wait_until="domcontentloaded",
            )

            # Wait for either English description container (.lang-en) or statement
            try:
                page.wait_for_selector(".lang-en", timeout=15000)
            except PWTimeout:
                logger.warning(".lang-en selector not found, falling back to page content")

            # Extract title
            title_el = page.query_selector("h1, .h2, title")
            title = (
                title_el.inner_text().strip()
                if title_el
                else f"ABC {contest_num} Task {task_char.upper()}"
            )
            # Clean up prefix like "B - " or "ABC300 - " from the title
            if " - " in title:
                title = title.split(" - ", 1)[1]

            # Extract description (prefer English section)
            lang_en_el = page.query_selector(".lang-en")
            if lang_en_el:
                description = lang_en_el.inner_text().strip()
            else:
                statement_el = page.query_selector("#task-statement, #main-container, main")
                description = (
                    statement_el.inner_text().strip()
                    if statement_el
                    else f"Problem statement unavailable. Visit: {problem_url}"
                )

            difficulty = "Easy" if task_char == "b" else "Medium"

            return {
                "title": title,
                "url": problem_url,
                "difficulty": difficulty,
                "description": description,
                "platform": "atcoder",
                "contest_id": contest_id,
                "problem_id": problem_id,
                "task_char": task_char,
            }

        except Exception as exc:
            raise RuntimeError(f"Failed to fetch AtCoder problem {problem_url}: {exc}") from exc
        finally:
            browser.close()
