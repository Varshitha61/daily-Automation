"""
platforms/submitters/codeforces_submit.py — Codeforces solution submitter using Playwright.

Submits an AI-generated solution to Codeforces by logging in via the web form and submitting.

Flow:
  1. Launch Playwright browser
  2. Log in using CODEFORCES_HANDLE and CODEFORCES_PASSWORD
  3. Navigate to problem submit page
  4. Fill in the code and submit
  5. Wait for the verdict on the status page
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

# Path to the persistent browser profile
_STORAGE_STATE_PATH: Path = (
    Path(__file__).parent.parent.parent / "logs" / "codeforces_session.json"
)

_WAIT_MS: int = 45_000

# Codeforces compiler IDs
_LANG_MAP = {
    "cpp":    "54",   # GNU G++17 7.3.0
    "c++":    "54",
    "python": "31",   # Python 3
    "java":   "36",   # Java 8
    "c":      "43",   # GNU GCC C11 5.1.0
}


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


def login(page: Page):
    """Log in to Codeforces."""
    logger.info("Logging in to Codeforces as '%s'...", Config.CODEFORCES_HANDLE)
    page.goto("https://codeforces.com/enter", timeout=_WAIT_MS, wait_until="domcontentloaded")

    # If already logged in, the handle will appear in the top right
    try:
        page.wait_for_selector(f"a:has-text('{Config.CODEFORCES_HANDLE}')", timeout=5_000)
        logger.info("Codeforces already logged in.")
        return
    except PWTimeout:
        pass

    try:
        page.fill("input#handleOrEmail", Config.CODEFORCES_HANDLE)
        page.fill("input#password", Config.CODEFORCES_PASSWORD)
        page.click("input[type='submit'][value='Login']")
        page.wait_for_selector(f"a:has-text('{Config.CODEFORCES_HANDLE}')", timeout=15_000)
        logger.info("Codeforces login successful.")
        
        # Save state
        _STORAGE_STATE_PATH.parent.mkdir(parents=True, exist_ok=True)
        page.context.storage_state(path=str(_STORAGE_STATE_PATH))
    except Exception as exc:
        raise RuntimeError(f"Codeforces login failed: {exc}")


def submit_solution(problem: dict, code: str) -> dict:
    """
    Submit a solution to Codeforces via Playwright.

    Args:
        problem (dict): Must contain 'contest_id' and 'index'.
        code    (str): Full source code to submit.

    Returns:
        dict: {
            "verdict":       str,
            "submission_id": str,
            "url":           str,
            "accepted":      bool,
            "runtime":       str,
            "memory":        str,
        }
    """
    contest_id = str(problem.get("contest_id", ""))
    index      = str(problem.get("index", ""))
    title      = problem.get("title", "unknown")
    lang_id    = _LANG_MAP.get(Config.SOLUTION_LANGUAGE.lower(), "54")

    if not contest_id or not index:
        raise RuntimeError(
            f"Codeforces submitter: missing 'contest_id' or 'index' in problem dict: {problem}"
        )

    logger.info("Submitting '%s' [%s%s] to Codeforces...", title, contest_id, index)

    submit_url = f"https://codeforces.com/contest/{contest_id}/submit/{index}"

    with sync_playwright() as p:
        browser = p.chromium.launch(
            headless=True,
            args=["--disable-blink-features=AutomationControlled"]
        )
        context = _build_context(browser)
        page = context.new_page()

        try:
            login(page)
            
            page.goto(submit_url, timeout=_WAIT_MS, wait_until="domcontentloaded")
            
            # Select language
            try:
                page.select_option("select[name='programTypeId']", value=lang_id)
            except Exception as e:
                logger.warning("Could not select language ID %s: %s", lang_id, e)
                
            # Click toggle to plain text editor if toggle exists
            try:
                toggle = page.locator("a#toggleEditorCheckbox")
                if toggle.count() > 0 and "toggle-on" in toggle.get_attribute("class"):
                    toggle.click()
            except Exception:
                pass

            # Inject code
            try:
                page.fill("textarea#sourceCodeTextarea", code, timeout=10_000)
            except Exception as e:
                logger.warning("Failed to fill sourceCodeTextarea: %s", e)
                # Fallback to copy-paste
                page.evaluate("async (text) => await navigator.clipboard.writeText(text)", code)
                page.click("textarea#sourceCodeTextarea")
                page.keyboard.press("Control+V")
                time.sleep(1)
                
            # Submit
            page.click("input[type='submit'][value='Submit']")
            
            # Wait for redirect to status page
            try:
                page.wait_for_url("**/my**", timeout=15_000)
            except PWTimeout:
                logger.warning("Codeforces submit redirect timeout. Continuing to check verdict...")
                
            # Poll for verdict in the table
            submission_id = "unknown"
            verdict = "Pending"
            accepted = False
            runtime = ""
            memory_str = ""
            sub_url = ""
            
            # Go to my submissions page for the contest
            status_url = f"https://codeforces.com/contest/{contest_id}/my"
            if not page.url.startswith(status_url):
                page.goto(status_url, timeout=_WAIT_MS, wait_until="domcontentloaded")
            
            for attempt in range(1, 20):
                try:
                    # Find the first row in the status table
                    row = page.locator("table.status-frame-datatable tr[data-submission-id]").first
                    row.wait_for(timeout=10_000)
                    
                    submission_id = row.get_attribute("data-submission-id")
                    sub_url = f"https://codeforces.com/contest/{contest_id}/submission/{submission_id}"
                    
                    verdict_cell = row.locator("td.status-verdict-cell")
                    verdict_text = verdict_cell.inner_text().strip()
                    
                    if not verdict_text or "Running" in verdict_text or "In queue" in verdict_text or "Pending" in verdict_text:
                        time.sleep(3)
                        page.reload()
                        continue
                        
                    verdict = verdict_text.splitlines()[0] if verdict_text else "Unknown"
                    accepted = ("Accepted" in verdict or "Happy New Year" in verdict)
                    
                    try:
                        time_cell = row.locator("td.time-consumed-cell").inner_text().strip()
                        mem_cell = row.locator("td.memory-consumed-cell").inner_text().strip()
                        runtime = time_cell
                        memory_str = mem_cell
                    except Exception:
                        pass
                        
                    logger.info("Codeforces verdict: %s | Runtime: %s | Memory: %s", verdict, runtime, memory_str)
                    break
                except Exception as e:
                    logger.warning("Error reading Codeforces verdict: %s", e)
                    time.sleep(3)
                    page.reload()
                    
            return {
                "verdict":       verdict,
                "submission_id": submission_id,
                "url":           sub_url,
                "accepted":      accepted,
                "runtime":       runtime,
                "memory":        memory_str,
            }

        finally:
            context.close()
            browser.close()
