import asyncio
from playwright.async_api import async_playwright

async def run():
    async with async_playwright() as p:
        browser = await p.chromium.launch(headless=True)
        page = await browser.new_page()
        await page.goto("https://www.codechef.com/practice/logical-problems", wait_until="networkidle")
        html = await page.content()
        links = [a.split('"')[0] for a in html.split('href="') if '/problems/' in a]
        print("Problems:", list(set(links))[:10])
        await browser.close()

if __name__ == "__main__":
    asyncio.run(run())
