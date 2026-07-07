import asyncio
import os
import sys

# Add project root to sys.path so config can be imported
sys.path.append(os.path.dirname(os.path.abspath(__file__)))
from config import Config
Config.validate()

from playwright.async_api import async_playwright

async def main():
    async with async_playwright() as p:
        browser = await p.chromium.launch(headless=True, args=["--disable-blink-features=AutomationControlled"])
        user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36"
        context = await browser.new_context(user_agent=user_agent)
        page = await context.new_page()
        try:
            await page.goto("https://www.hackerrank.com/auth/login", wait_until="domcontentloaded")
            await page.fill("input[name='username']", Config.HACKERRANK_USERNAME)
            await page.fill("input[name='password']", Config.HACKERRANK_PASSWORD)
            await page.click("button[data-analytics='LoginPassword'], button[type='submit']")
            await page.wait_for_url(lambda url: "auth/login" not in url, timeout=15000)
            await asyncio.sleep(5) # wait for dashboard to populate
            await page.screenshot(path="hackerrank_dashboard.png")
            html = await page.content()
            with open("hackerrank_dashboard.html", "w", encoding="utf-8") as f:
                f.write(html)
            print("Screenshot and HTML saved for hackerrank.")
        except Exception as e:
            print("Error:", e)
        finally:
            await browser.close()

asyncio.run(main())
