import json
from pathlib import Path

cookies = [
    {
        "name": "cf_clearance",
        "value": "bJr5vBftxCuij60ya4PNCj1Vv1yfvfzcJjXP6pjDtvU-1783574250-1.2.1.1-3dpqjVEcfDHC8ybr0S.3F0KxGtdKiXYPXucYg.TYJOa.wYD7eII9ZSSOUmTLuXMi0NRBaphnet5bSgaBGWVy.Dw9ND1wStZc03HiUF6amUeVQeXNtm4iPiFmg_sK6p83pXqOHSC5a3SCi06ZjapK55Ok7t1Z9hMxrHnfSFofnhnk50PBUr7x_Qaa7H5UF1rBsOdTlgyRnk7c3cZ2nFTKHpjvdC9_NR2yWzS.jKrawJhrXdbD_eTm7m21cenfyi6xbEnxeitP55OJSDgbq66LRLTiIjK_9raKBit7U_fLkJJb2LjOuUJyHaRU9Ym2GgY0V_SYQkFDzabRGlyz6PtV1I1k6j.9jiZG91tHiVrFhoPQDsLlSFPqqfcrkf5JpdBZQBzLiGYBxVTrXaHrEjTQi5s_tCsLE35v.PeXhnuInzrU8NlNx_1Yq3ege04U71pa",
        "domain": ".codeforces.com",
        "path": "/",
        "expires": -1,
        "httpOnly": False,
        "secure": True,
        "sameSite": "None",
    },
    {
        "name": "JSESSIONID",
        "value": "8E4AC1370F23ECAB284B9AA4A8BAC1D8",
        "domain": "codeforces.com",
        "path": "/",
        "expires": -1,
        "httpOnly": True,
        "secure": False,
        "sameSite": "Lax",
    },
    {
        "name": "39ce7",
        "value": "CFWFo9bA",
        "domain": "codeforces.com",
        "path": "/",
        "expires": -1,
        "httpOnly": False,
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
