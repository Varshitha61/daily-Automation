import requests
import json
from config import Config

Config.validate()
url = f"https://api.telegram.org/bot{Config.TELEGRAM_BOT_TOKEN}/sendMessage"
payload = {
    "chat_id": Config.TELEGRAM_CHAT_ID,
    "text": "<b>Test Message</b> from Bot",
    "parse_mode": "HTML",
    "disable_web_page_preview": True,
}
resp = requests.post(url, json=payload)
print(resp.status_code)
print(resp.text)
