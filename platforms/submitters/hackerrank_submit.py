"""
platforms/submitters/hackerrank_submit.py — HackerRank solution submitter using Playwright.

Reuses the authenticated browser session created by the fetcher to navigate to the 
problem page, inject the generated code, and submit it.
"""

import logging
import time
from typing import Optional
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

_WAIT_MS: int = 45_000


def _build_context(browser: Browser) -> BrowserContext:
    user_agent = (
        "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
        "AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/124.0.0.0 Safari/537.36"
    )

    if _STORAGE_STATE_PATH.exists():
        return browser.new_context(
            storage_state=str(_STORAGE_STATE_PATH),
            user_agent=user_agent,
            permissions=["clipboard-read", "clipboard-write"],
        )

    return browser.new_context(
        user_agent=user_agent,
        permissions=["clipboard-read", "clipboard-write"]
    )


def submit_solution(problem: dict, code: str) -> dict:
    """
    Submit a solution to HackerRank via Playwright.

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
        browser = p.chromium.launch(headless=True)
        context = _build_context(browser)
        page = context.new_page()

        try:
            page.goto(url, timeout=_WAIT_MS, wait_until="domcontentloaded")
            
            # Wait for the editor to load
            try:
                page.wait_for_selector(".view-lines, .inputarea, .CodeMirror", timeout=20_000)
            except PWTimeout:
                logger.warning("Could not find HackerRank editor on the page.")

            # Inject the code into the editor.
            textarea = page.locator(".inputarea").first
            if textarea.count() > 0:
                textarea.click()
                page.keyboard.press("Control+A")
                page.keyboard.press("Backspace")
                
                # Copy code to clipboard and paste
                page.evaluate("async (text) => await navigator.clipboard.writeText(text)", code)
                page.keyboard.press("Control+V")
                time.sleep(1)
            else:
                logger.warning("Could not find HackerRank editor textarea.")

            # Click Submit Code
            try:
                submit_btn = page.locator("button:has-text('Submit Code')")
                submit_btn.click()
            except Exception as e:
                logger.warning("Failed to click 'Submit Code': %s", e)
            
            # Wait for verdict
            verdict = "Submitted"
            accepted = False
            
            try:
                status_locator = page.locator(".test-case-status, .compiler-message, .status-title")
                status_locator.first.wait_for(timeout=45_000)
                verdict = status_locator.first.inner_text()
                accepted = ("Congratulations" in verdict or "Accepted" in verdict or "Success" in verdict)
            except PWTimeout:
                logger.warning("Timed out waiting for verdict on HackerRank.")
                verdict = "Unknown Verdict (Timed Out)"

            return {
                "verdict": verdict,
                "accepted": accepted,
                "url": page.url,
            }

        finally:
            context.close()
            browser.close()
