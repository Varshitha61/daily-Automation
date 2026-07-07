"""
platforms/submitters/codechef_submit.py — CodeChef solution submitter using Playwright.

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

# Path to the persistent browser profile created by codechef.py
_STORAGE_STATE_PATH: Path = (
    Path(__file__).parent.parent.parent / "logs" / "codechef_session.json"
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
    Submit a solution to CodeChef via Playwright.

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

    with sync_playwright() as p:
        browser = p.chromium.launch(headless=True)
        context = _build_context(browser)
        page = context.new_page()

        try:
            page.goto(url, timeout=_WAIT_MS, wait_until="domcontentloaded")
            
            # Click Submit button to enter submit mode if needed
            try:
                page.wait_for_selector("button:has-text('Submit')", timeout=15_000)
            except PWTimeout:
                try:
                    page.click("button:has-text('Solve')", timeout=5_000)
                    page.wait_for_selector("button:has-text('Submit')", timeout=15_000)
                except PWTimeout:
                    pass

            # Inject the code into the editor. 
            # CodeChef uses Monaco Editor. Best way is to clear and copy-paste.
            textarea = page.locator(".inputarea")
            if textarea.count() > 0:
                textarea.first.click()
                page.keyboard.press("Control+A")
                page.keyboard.press("Backspace")
                
                # Copy code to clipboard and paste
                page.evaluate("async (text) => await navigator.clipboard.writeText(text)", code)
                page.keyboard.press("Control+V")
                time.sleep(1) # Let the editor register the paste
            else:
                logger.warning("Could not find CodeChef editor textarea.")

            # Click the actual Submit button
            submit_btn = page.locator("button:has-text('Submit')").last
            submit_btn.click()
            
            # Wait for verdict
            verdict = "Submitted"
            accepted = False
            
            try:
                status_locator = page.locator("text='Correct Answer'").or_(page.locator("text='Wrong Answer'")).or_(page.locator("text='Time Limit Exceeded'")).or_(page.locator("text='Compilation Error'"))
                status_locator.first.wait_for(timeout=40_000)
                verdict = status_locator.first.inner_text()
                accepted = ("Correct Answer" in verdict)
            except PWTimeout:
                logger.warning("Timed out waiting for verdict on CodeChef.")
                verdict = "Unknown Verdict (Timed Out)"

            return {
                "verdict": verdict,
                "accepted": accepted,
                "url": page.url,
            }

        finally:
            context.close()
            browser.close()
