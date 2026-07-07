"""
notifier/telegram.py — Telegram notification sender for the Daily Coding Bot.

Uses the Telegram Bot HTTP API directly via the requests library.  No
third-party Telegram SDK is required.

Credentials required in .env:
    TELEGRAM_BOT_TOKEN — from @BotFather on Telegram
    TELEGRAM_CHAT_ID   — numeric chat ID (use @userinfobot to find yours)

Usage:
    from notifier.telegram import send_solution_notification, send_error_notification
    send_solution_notification("leetcode", "Two Sum", "solutions/...", code_str)
    send_error_notification("codeforces", "Rate limit exceeded")
"""

import logging
import time
from datetime import datetime
from typing import Optional

import requests

from config import Config

logger = logging.getLogger(__name__)

_API_BASE: str = "https://api.telegram.org"
_MAX_MESSAGE_LENGTH: int = 4096   # Telegram's hard cap
_PREVIEW_LINES: int = 20          # Lines of code to show in the notification


# ---------------------------------------------------------------------------
# Core sender
# ---------------------------------------------------------------------------

def send_message(text: str, parse_mode: str = "HTML") -> bool:
    """
    Send a text message to the configured Telegram chat.

    Retries once after 3 seconds on a network error or non-2xx response.

    Args:
        text       (str): The message body.  May contain HTML tags when
                          parse_mode is "HTML".
        parse_mode (str): Telegram parse mode — "HTML" or "Markdown".
                          Defaults to "HTML".

    Returns:
        bool: True if the message was delivered successfully; False if both
              attempts failed (the error is logged but not re-raised, so the
              bot continues running).
    """
    url = f"{_API_BASE}/bot{Config.TELEGRAM_BOT_TOKEN}/sendMessage"
    payload = {
        "chat_id": Config.TELEGRAM_CHAT_ID,
        "text": _truncate(text),
        "parse_mode": parse_mode,
        "disable_web_page_preview": True,
    }

    for attempt in range(1, 3):
        try:
            logger.debug("Sending Telegram message — attempt %d/2", attempt)
            response = requests.post(
                url,
                json=payload,
                timeout=Config.REQUEST_TIMEOUT,
            )
            response.raise_for_status()
            result = response.json()

            if not result.get("ok"):
                raise RuntimeError(
                    f"Telegram API returned ok=false: {result.get('description')} - {response.text}"
                )

            logger.info("Telegram message delivered (attempt %d).", attempt)
            return True

        except (requests.RequestException, RuntimeError) as exc:
            err_text = exc.response.text if hasattr(exc, "response") and exc.response else ""
            logger.warning(
                "Telegram send failed on attempt %d: %s. Response: %s", attempt, exc, err_text
            )
            if attempt < 2:
                logger.info("Retrying Telegram send in 3 seconds…")
                time.sleep(3)

    logger.error("Telegram message delivery failed after 2 attempts.")
    return False


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _truncate(text: str) -> str:
    """
    Truncate a message to Telegram's maximum allowed length.

    Appends an ellipsis note if the text was cut.

    Args:
        text (str): The original message text.

    Returns:
        str: The message, truncated to _MAX_MESSAGE_LENGTH characters if
             necessary.
    """
    if len(text) <= _MAX_MESSAGE_LENGTH:
        return text
    truncation_note = "\n\n<i>… message truncated to fit Telegram's 4096-char limit.</i>"
    cutoff = _MAX_MESSAGE_LENGTH - len(truncation_note)
    return text[:cutoff] + truncation_note


def _code_preview(code: str, max_lines: int = _PREVIEW_LINES) -> str:
    """
    Return the first *max_lines* lines of a code string.

    Args:
        code      (str): Raw source code.
        max_lines (int): Maximum number of lines to include.

    Returns:
        str: The first max_lines lines joined as a single string, with a
             trailing note if lines were omitted.
    """
    lines = code.splitlines()
    preview = "\n".join(lines[:max_lines])
    if len(lines) > max_lines:
        preview += f"\n… (+{len(lines) - max_lines} more lines)"
    return preview


def _now_str() -> str:
    """
    Return the current local datetime formatted for display in notifications.

    Returns:
        str: e.g. "2024-01-15 08:03:42"
    """
    return datetime.now().strftime("%Y-%m-%d %H:%M:%S")


# ---------------------------------------------------------------------------
# Formatted notifications
# ---------------------------------------------------------------------------

def send_solution_notification(
    platform: str,
    title: str,
    file_path: str,
    code: str,
    verdict: str = "Saved Locally",
    submission_url: str = "",
    runtime: str = "",
    memory: str = "",
) -> bool:
    """
    Send a rich success notification to Telegram after a problem is solved.

    The message includes the platform name, problem title, timestamp, saved
    file path, verdict, and a preview of the first 20 lines of the generated solution.

    Args:
        platform       (str): Platform identifier, e.g. "leetcode".
        title          (str): The problem title.
        file_path      (str): Path where the solution file was saved.
        code           (str): The full solution code string.
        verdict        (str): Submission verdict (e.g. "Accepted").
        submission_url (str): Link to the submission.
        runtime        (str): Runtime string (e.g. "45 ms").
        memory         (str): Memory string (e.g. "14.2 MB").

    Returns:
        bool: True if the notification was delivered; False on failure.
    """
    preview = _code_preview(code)
    # Escape HTML special chars in the code preview so Telegram renders it safely
    safe_preview = preview.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")
    safe_path = file_path.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")
    safe_title = title.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")
    
    verdict_emoji = "✅" if verdict == "Accepted" else "⚠️" if verdict != "Saved Locally" else "💾"

    message = (
        f"✅ <b>{platform.upper()}</b> Daily Problem Solved\n"
        f"\n"
        f"📌 <b>Problem:</b> {safe_title}\n"
        f"🕐 <b>Time:</b> {_now_str()}\n"
        f"{verdict_emoji} <b>Verdict:</b> {verdict}\n"
    )
    
    if submission_url:
        message += f"🔗 <b>Submission:</b> <a href=\"{submission_url}\">View Details</a>\n"
    if runtime or memory:
        message += f"🚀 <b>Stats:</b> {runtime or 'N/A'} | {memory or 'N/A'}\n"
        
    message += (
        f"💾 <b>Saved to:</b> <code>{safe_path}</code>\n"
        f"\n"
        f"<b>Solution Preview (first {_PREVIEW_LINES} lines):</b>\n"
        f"<pre>{safe_preview}</pre>"
    )

    logger.info(
        "Sending solution notification for '%s' [%s].", title, platform
    )
    return send_message(message)


def send_error_notification(platform: str, error_message: str) -> bool:
    """
    Send a failure alert to Telegram when a platform step raises an exception.

    Args:
        platform      (str): Platform identifier, e.g. "codeforces".
        error_message (str): The exception message or error description.

    Returns:
        bool: True if the notification was delivered; False on failure.
    """
    safe_error = (
        error_message
        .replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;")
    )

    message = (
        f"❌ <b>{platform.upper()}</b> Failed\n"
        f"\n"
        f"🔴 <b>Error:</b> {safe_error}\n"
        f"🕐 <b>Time:</b> {_now_str()}"
    )

    logger.error(
        "Sending error notification for platform '%s': %s",
        platform,
        error_message,
    )
    return send_message(message)


def send_daily_summary(succeeded: list[str], failed: list[str]) -> bool:
    """
    Send an end-of-run summary listing which platforms succeeded and which
    failed.

    Args:
        succeeded (list[str]): Platform names that completed successfully.
        failed    (list[str]): Platform names that encountered errors.

    Returns:
        bool: True if the summary message was delivered; False on failure.
    """
    ok_lines = "\n".join(f"  ✅ {p.upper()}" for p in succeeded) or "  (none)"
    fail_lines = "\n".join(f"  ❌ {p.upper()}" for p in failed) or "  (none)"

    message = (
        f"📊 <b>Daily Bot Run Complete</b>\n"
        f"🕐 <b>Time:</b> {_now_str()}\n"
        f"\n"
        f"<b>Succeeded:</b>\n{ok_lines}\n"
        f"\n"
        f"<b>Failed:</b>\n{fail_lines}"
    )

    logger.info("Sending daily summary — succeeded=%s, failed=%s", succeeded, failed)
    return send_message(message)
