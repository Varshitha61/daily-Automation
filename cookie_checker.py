"""
cookie_checker.py — Checks expiry of all platform session cookies/tokens
and sends Telegram warnings before they expire.

Checked on every bot run (called from main.py before the platform pipeline).

Cookies/tokens checked:
  - LeetCode SESSION   (JWT — decode exp claim)
  - LeetCode 2 SESSION (JWT — decode exp claim)
  - CodeChef AUTH_TOKEN (JWT — decode exp claim)
  - Codeforces cf_clearance (expires ~1-2 days, checked by age heuristic)
  - Codeforces 39ce7 / JSESSIONID (no exp embedded — warn if missing)
"""

import base64
import json
import logging
import time
from datetime import datetime, timezone
from typing import Optional

from config import Config
from notifier import telegram

logger = logging.getLogger(__name__)

# Warn if a cookie expires within this many days
_WARN_DAYS = 3


# ---------------------------------------------------------------------------
# JWT decoder (no external library needed — just base64 the payload)
# ---------------------------------------------------------------------------

def _decode_jwt_exp(token: str) -> Optional[datetime]:
    """
    Decode the `exp` claim from a JWT token without verifying the signature.

    Returns the expiry as a timezone-aware UTC datetime, or None if the token
    cannot be decoded or has no `exp` claim.
    """
    try:
        parts = token.strip().split(".")
        if len(parts) != 3:
            return None
        # Pad base64 to a multiple of 4
        payload_b64 = parts[1] + "=" * (-len(parts[1]) % 4)
        payload = json.loads(base64.urlsafe_b64decode(payload_b64).decode("utf-8"))
        exp = payload.get("exp")
        if exp is None:
            return None
        return datetime.fromtimestamp(int(exp), tz=timezone.utc)
    except Exception as e:
        logger.debug("Could not decode JWT exp: %s", e)
        return None


# ---------------------------------------------------------------------------
# Individual cookie checks
# ---------------------------------------------------------------------------

def _check_jwt(name: str, token: str) -> Optional[str]:
    """
    Check a JWT token's expiry.

    Returns a warning string if the token is expired or expiring soon,
    or None if the token is healthy.
    """
    if not token or not token.strip():
        return f"⚠️ <b>{name}</b> is <b>not set</b> — platform will be skipped."

    exp = _decode_jwt_exp(token)
    if exp is None:
        # Not a JWT or no exp claim — can't check, skip silently
        return None

    now = datetime.now(tz=timezone.utc)
    diff = exp - now
    days_left = diff.total_seconds() / 86400

    if days_left < 0:
        return (
            f"🔴 <b>{name}</b> has <b>EXPIRED</b> "
            f"(expired {abs(diff.days)} day(s) ago on "
            f"{exp.strftime('%Y-%m-%d %H:%M UTC')}).\n"
            f"   ➜ Please refresh the cookie and update your GitHub Secret."
        )
    elif days_left <= _WARN_DAYS:
        return (
            f"🟡 <b>{name}</b> expires in <b>{diff.days} day(s)</b> "
            f"({exp.strftime('%Y-%m-%d %H:%M UTC')}).\n"
            f"   ➜ Refresh it soon to avoid bot failures."
        )
    else:
        logger.debug("%s is healthy — expires in %.1f days.", name, days_left)
        return None


def _check_cf_clearance(name: str, value: str) -> Optional[str]:
    """
    Check Codeforces cf_clearance cookie.
    The expiry timestamp is embedded in the value itself (format: ...-<unix_ts>-...).
    """
    if not value or not value.strip():
        return (
            f"⚠️ <b>{name}</b> is <b>not set</b>.\n"
            f"   ➜ Get it from DevTools → Application → Cookies → codeforces.com → cf_clearance\n"
            f"   ➜ Add as GitHub Secret: CODEFORCES_CF_CLEARANCE"
        )

    try:
        # cf_clearance format: <random>-<unix_timestamp>-<version>-...
        parts = value.split("-")
        # Find the unix timestamp part (10 digits)
        exp_ts = None
        for part in parts:
            if len(part) == 10 and part.isdigit():
                exp_ts = int(part)
                break

        if exp_ts is None:
            logger.debug("Could not parse cf_clearance timestamp from value.")
            return None

        exp = datetime.fromtimestamp(exp_ts, tz=timezone.utc)
        now = datetime.now(tz=timezone.utc)
        diff = exp - now
        days_left = diff.total_seconds() / 86400

        if days_left < 0:
            return (
                f"🔴 <b>{name}</b> has <b>EXPIRED</b> "
                f"(expired {abs(diff.days)} day(s) ago).\n"
                f"   ➜ Get a fresh cf_clearance from your browser and update the GitHub Secret."
            )
        elif days_left <= _WARN_DAYS:
            return (
                f"🟡 <b>{name}</b> expires in <b>{diff.days} day(s)</b> "
                f"({exp.strftime('%Y-%m-%d %H:%M UTC')}).\n"
                f"   ➜ Refresh it soon — Cloudflare bypass will stop working."
            )
        else:
            logger.debug("%s is healthy — expires in %.1f days.", name, days_left)
            return None

    except Exception as e:
        logger.debug("Could not check cf_clearance expiry: %s", e)
        return None


def _check_plain_cookie(name: str, value: str) -> Optional[str]:
    """
    Check a plain (non-JWT) session cookie — just warn if it's missing.
    """
    if not value or not value.strip():
        return (
            f"⚠️ <b>{name}</b> is <b>not set</b>.\n"
            f"   ➜ Add it to your GitHub Secrets."
        )
    return None


# ---------------------------------------------------------------------------
# Main check runner
# ---------------------------------------------------------------------------

def check_all_cookies() -> list[str]:
    """
    Run expiry checks on all platform cookies/tokens.

    Returns:
        list[str]: A list of warning messages (HTML-formatted).
                   Empty list means all cookies are healthy.
    """
    warnings: list[str] = []

    checks = [
        # LeetCode JWTs
        lambda: _check_jwt("LEETCODE_SESSION (Account 1)",    Config.LEETCODE_SESSION),
        lambda: _check_jwt("LEETCODE_SESSION (Account 2)",    Config.LEETCODE_2_SESSION),

        # CodeChef JWT
        lambda: _check_jwt("CODECHEF_AUTH_TOKEN",             Config.CODECHEF_AUTH_TOKEN),

        # Codeforces Cloudflare bypass
        lambda: _check_cf_clearance("CODEFORCES_CF_CLEARANCE", Config.CODEFORCES_CF_CLEARANCE),

        # Codeforces session cookies (plain — just check presence)
        lambda: _check_plain_cookie("CODEFORCES_39CE7",        Config.CODEFORCES_39CE7),
        lambda: _check_plain_cookie("CODEFORCES_JSESSIONID",   Config.CODEFORCES_JSESSIONID),
    ]

    for check_fn in checks:
        try:
            result = check_fn()
            if result:
                warnings.append(result)
        except Exception as e:
            logger.warning("Cookie check error: %s", e)

    return warnings


def send_cookie_warnings() -> None:
    """
    Check all cookies and send a Telegram warning message if any are
    expired or expiring within the next 3 days.

    Called automatically at the start of each bot run.
    """
    logger.info("Checking cookie/token expiry…")
    warnings = check_all_cookies()

    if not warnings:
        logger.info("All cookies are healthy — no expiry warnings.")
        return

    warning_lines = "\n\n".join(warnings)
    message = (
        f"🍪 <b>Cookie Expiry Warning</b>\n"
        f"🕐 <b>Checked at:</b> {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n"
        f"\n"
        f"The following cookies need attention:\n\n"
        f"{warning_lines}\n\n"
        f"<i>Update the values in GitHub → Settings → Secrets → Actions</i>"
    )

    logger.warning("Cookie warnings: %d issue(s) found.", len(warnings))
    telegram.send_message(message)
