"""
export_cf_cookies.py -- Generate CF_COOKIES_JSON from your real browser cookies.

Playwright's browser is fingerprinted and blocked by Cloudflare even in
non-headless mode, so we cannot use it to log in.  Instead, this script:

  1. Tells you exactly which cookies to copy from Chrome/Firefox DevTools
  2. Asks you to paste each value
  3. Formats them into a valid Playwright storage-state JSON
  4. Saves to logs/codeforces_session.json  AND  prints the GitHub secret value

Usage:
    python export_cf_cookies.py
"""

import json
import sys
import webbrowser
from pathlib import Path

SESSION_PATH = Path(__file__).parent / "logs" / "codeforces_session.json"


def ask(prompt: str) -> str:
    try:
        return input(prompt).strip()
    except KeyboardInterrupt:
        print("\nAborted.")
        sys.exit(0)


def main():
    sep = "=" * 62
    print(sep)
    print("  Codeforces Cookie Exporter")
    print(sep)
    print()
    print("We will copy cookies from YOUR REAL BROWSER (not Playwright).")
    print()
    print("Step 1: Opening codeforces.com in your default browser...")
    webbrowser.open("https://codeforces.com")
    print()
    print("Make sure you are LOGGED IN to Codeforces.")
    print("(If not, log in now, then come back here.)")
    print()
    input("Press ENTER when you are logged in and on codeforces.com...")

    print()
    print("-" * 62)
    print("Step 2: Open DevTools in your browser:")
    print()
    print("  Chrome/Edge : F12  ->  Application  ->  Cookies")
    print("                ->  https://codeforces.com")
    print()
    print("  Firefox     : F12  ->  Storage  ->  Cookies")
    print("                ->  https://codeforces.com")
    print("-" * 62)
    print()
    print("You will see a list of cookies.  We need these values:")
    print()
    print("  1. JSESSIONID  (if present)")
    print("  2. 39ce7        (Codeforces session cookie)")
    print("  3. Codeforces_csrf_token  OR  _csrf  (CSRF token cookie)")
    print("  4. X-User-Sha1  (if present)")
    print()
    print("Copy each cookie VALUE (not the name) from the DevTools panel.")
    print("Leave blank and press ENTER to skip a cookie you cannot find.")
    print()
    input("Press ENTER when DevTools is open and you can see the cookies...")
    print()

    cookies = []

    def collect(name: str, domain: str = ".codeforces.com", path: str = "/",
                http_only: bool = True, secure: bool = True):
        value = ask(f"  Paste value of  [{name}]  (or ENTER to skip): ")
        if value:
            cookies.append({
                "name": name,
                "value": value,
                "domain": domain,
                "path": path,
                "expires": -1,
                "httpOnly": http_only,
                "secure": secure,
                "sameSite": "Lax",
            })
            print(f"    OK - {name} saved ({len(value)} chars)")
        else:
            print(f"    Skipped {name}")

    print()
    print("-- Paste cookie values below --")
    print()
    collect("JSESSIONID")
    collect("39ce7")
    collect("Codeforces_csrf_token", http_only=False)
    collect("_csrf", http_only=False)
    collect("X-User-Sha1", http_only=False)

    # Ask for any extra cookies the user sees
    print()
    print("Any OTHER Codeforces cookies you want to include?")
    print("(Enter the cookie NAME, then its VALUE; blank name = done)")
    while True:
        name = ask("  Extra cookie name  (ENTER to finish): ")
        if not name:
            break
        value = ask(f"  Value of [{name}]: ")
        if value:
            cookies.append({
                "name": name,
                "value": value,
                "domain": ".codeforces.com",
                "path": "/",
                "expires": -1,
                "httpOnly": False,
                "secure": True,
                "sameSite": "Lax",
            })
            print(f"    OK - {name} saved")

    if not cookies:
        print()
        print("ERROR: No cookies were entered. Cannot generate session JSON.")
        print("Please re-run the script and paste at least the '39ce7' cookie value.")
        sys.exit(1)

    # Build Playwright storage-state format
    state = {
        "cookies": cookies,
        "origins": [],
    }
    state_json = json.dumps(state, indent=2)

    # Save locally
    SESSION_PATH.parent.mkdir(parents=True, exist_ok=True)
    SESSION_PATH.write_text(state_json, encoding="utf-8")

    print()
    print(sep)
    print("  SUCCESS! Session JSON generated.")
    print(sep)
    print()
    print(f"Saved locally to: {SESSION_PATH}")
    print()
    print("Copy EVERYTHING between the dashes and paste it into your")
    print("GitHub secret named  CF_COOKIES_JSON :")
    print()
    print("GitHub: Repo -> Settings -> Secrets -> Actions -> New secret")
    print("  Name:  CF_COOKIES_JSON")
    print("  Value: (paste the JSON below)")
    print()
    print("-" * 62)
    print(state_json)
    print("-" * 62)
    print()
    print("Done! Trigger a new GitHub Actions run to test.")
    print()


if __name__ == "__main__":
    main()
