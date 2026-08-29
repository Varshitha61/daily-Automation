"""
build_codechef_session.py
Creates logs/codechef_session.json from browser cookies so the bot
can authenticate to CodeChef without logging in via Playwright.
"""
import json
from pathlib import Path
from datetime import datetime, timezone

def to_ts(s):
    return datetime.fromisoformat(s.replace("Z", "+00:00")).timestamp()

cookies = [
    {
        "name": "Authorization",
        "value": "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJjb2RlY2hlZi5jb20iLCJzdWIiOiI0Mjg5MTQxIiwidXNlcm5hbWUiOiJ2YXJzaGl0aGEwNjEiLCJpYXQiOjE3ODc3NTU3NjAsIm5iZiI6MTc4Nzc1NTc2MCwiZXhwIjoxNzg5NzUwMTYwfQ.smgaLdAdKGAtSpEpPf1WE-ms75fMvtQAFtVriaoVS5Y",
        "domain": "www.codechef.com", "path": "/",
        "expires": to_ts("2026-09-18T18:22:56.362Z"),
        "httpOnly": True, "secure": True, "sameSite": "Lax",
    },
    {
        "name": "cf_clearance",
        "value": "Yd44SY5o8HnPGPNUaHt1YXmAYy5d474IqxU.ubONPFI-1785337052-1.2.1.1-4WjDsOboFODRpy9SfC7v.Qku5sYxqo7_Znxy.if.FEnSVRmrZBIGTsb6Y.bP1UCbUPq1S.EteiwPUil3aoXHFWt_AydcuTEErxW1wwi.gtRt4cJ9539HSEDHAJI3c2mj.PjpNr6AWMq2NHug9HjXggnq_Nb8zi6adaFmGX4lbdtLSIVJRuBZpR5UyO3caaTzRnfuYCFH9JRpX7rmRik79k3mhyOGI48kwGZWTLhjJWlcpoEw6E0tH2nn7lPrt7.wMmAVg1AnjaoJw6P1JjvveqWECGWl4o9ms8R1c1m29HNMoR0ueDpoRS2eRSvkfjhS3KupGC40YZmZb_IVOpLvnNzjZaCpo6Kk_KKu55FZzqEr0biImw7HNA4ntg4gT8TSET0V75nrueq37upOlL7dydXPvm7HJmyFWgBsRLrkF6rV2ejtYXMfYUWiaMLDs9NFwO4Ahn5oMZHAA09A9jGRWqOrjse6zCsZzY9vVp2NeFD_CetUWEyyjieWUiaEH4O8XTjiuCOoja4nF_bsebEhxfPyo2EL8jJF0Zg20JZNTWG_pMgcL.9_aNUMnTOgiOjCI38WvbuB3UaLNshqrISueg",
        "domain": ".codechef.com", "path": "/",
        "expires": to_ts("2027-07-29T14:58:14.772Z"),
        "httpOnly": True, "secure": True, "sameSite": "None",
    },
    {
        "name": "SESS93b6022d778ee317bf48f7dbffe03173",
        "value": "7fbb8bec0afa2e56d7828dafebc392d2",
        "domain": ".codechef.com", "path": "/",
        "expires": to_ts("2026-09-18T18:22:56.362Z"),
        "httpOnly": True, "secure": True, "sameSite": "Lax",
    },
    {
        "name": "uid",
        "value": "4289141",
        "domain": "www.codechef.com", "path": "/",
        "expires": to_ts("2026-09-18T18:22:56.362Z"),
        "httpOnly": False, "secure": False, "sameSite": "Lax",
    },
    {
        "name": "userkey",
        "value": "56a7ba633e70a409025ae6f4d029a938",
        "domain": "www.codechef.com", "path": "/",
        "expires": to_ts("2026-08-29T03:57:38.000Z"),
        "httpOnly": False, "secure": False, "sameSite": "Lax",
    },
]

state = {"cookies": cookies, "origins": []}

path = Path("logs/codechef_session.json")
path.parent.mkdir(exist_ok=True)
path.write_text(json.dumps(state, indent=2))
print(f"CodeChef session file created: {path}")
print(f"Cookies saved: {len(cookies)}")
print("The bot will now use these cookies instead of logging in.")
