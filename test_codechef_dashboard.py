import asyncio
import os
import sys

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
            await page.goto("https://www.codechef.com/login", wait_until="networkidle")
            await page.locator("form#ajax-login-form input[name='name']").fill(Config.CODECHEF_USERNAME, force=True)
            await page.locator("form#ajax-login-form input[name='pass']").fill(Config.CODECHEF_PASSWORD, force=True)
            await page.locator("input.cc-login-btn").click(force=True)
            await page.wait_for_url(lambda url: "login" not in url, timeout=10000)
            
            await page.goto("https://www.codechef.com/dashboard", wait_until="networkidle")
            html = await page.content()
            with open("codechef_dashboard.html", "w", encoding="utf-8") as f:
                f.write(html)
            print("Dashboard HTML saved for codechef.")
        except Exception as e:
            print("Error:", e)
        finally:
            await browser.close()

asyncio.run(main())
