import json
from pathlib import Path

cookies = [
    {
        "name": "39ce7",
        "value": "CFOErQQY",
        "domain": "codeforces.com",
        "path": "/",
        "expires": -1,
        "httpOnly": False,
        "secure": False,
        "sameSite": "Lax",
    },
    {
        "name": "JSESSIONID",
        "value": "2A475B36B2FC4CD1441D7C159DB11EED",
        "domain": "codeforces.com",
        "path": "/",
        "expires": -1,
        "httpOnly": True,
        "secure": False,
        "sameSite": "Lax",
    },
    {
        "name": "X-User-Sha1",
        "value": "42913c7eab1456df63cfce6ea78d44112c84e931",
        "domain": "codeforces.com",
        "path": "/",
        "expires": -1,
        "httpOnly": False,
        "secure": False,
        "sameSite": "Lax",
    },
    {
        "name": "70a7c28f3de",
        "value": "deq5illaalfsfk51li",
        "domain": ".codeforces.com",
        "path": "/",
        "expires": -1,
        "httpOnly": False,
        "secure": False,
        "sameSite": "Lax",
    },
    {
        "name": "cf_clearance",
        "value": "yFCmBTAY61ZPpFAqOXKexXa48DyB846jsFzVtPGIKA4-1783484263-1.2.1.1-VV8BeTEulfubvRJECZam1L29dwbsIzW3Rw.Hx31rutlEC5FRXcuzbYt_3s1HchN8vOZFD0XXtomuIvHdyaK63x2LHOYgTxDnLVZJYQ2MErg1gJS01PwuGdhwlaSn6FXw8OUoDvFAuwpyx_MAFo448vjNgYC3F5yGWuSlGwmDBz8Le3wMPyUKKQhloxAZiuyIsHyPv1MPj0OmPpbd5PHW1Pa_bpBElCmYuzlBpCi4bMonKZLD74s8FT1GwjgEooTeOMHHKRxY0nfld787SsVuaN1JKdA66584Y59jXcihlgJwqGl0qKp4p6qpmsqnyMC_G4lfhIAKnbhiKPyvXrG2xYZ9NXiIeJeeI.8H0aoNn3xwpz06Ax9d4klIHzbcTS8.gDSAXfB.T3u.zxT_HnZr6QKKLnEfT5Lv0BRzhRyJPijxfWnEji4.meyazMimsSL1D5PqkLfzA_bPXUm.9NUO0XwUw5Sa9WQUUXyvToXUwWL1htdoAQx6nOX6C8kdcF0Jr08IU4oVloe0e87CfLzH6A",
        "domain": ".codeforces.com",
        "path": "/",
        "expires": -1,
        "httpOnly": False,
        "secure": True,
        "sameSite": "None",
    },
]

state = {"cookies": cookies, "origins": []}
state_json = json.dumps(state, indent=2)

out = Path("logs/codeforces_session.json")
out.parent.mkdir(parents=True, exist_ok=True)
out.write_text(state_json, encoding="utf-8")

print("Saved to logs/codeforces_session.json")
print()
print("=== Paste this entire block as GitHub Secret CF_COOKIES_JSON ===")
print(state_json)
print("================================================================")
print()
print("Cookies included:")
for c in cookies:
    print("  " + c["name"] + " (" + str(len(c["value"])) + " chars)")
