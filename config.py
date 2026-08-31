"""
config.py — Centralised configuration loader for the Daily Coding Challenge Bot.

All credentials and runtime settings are read exclusively from the .env file
via python-dotenv.  No value is ever hardcoded here.

Usage:
    from config import Config
    Config.validate()          # call once at bot startup
    token = Config.GEMINI_API_KEY
"""

import os
import logging
from dotenv import load_dotenv

# Load the .env file from the project root (same directory as this file).
load_dotenv(dotenv_path=os.path.join(os.path.dirname(__file__), ".env"))

logger = logging.getLogger(__name__)


class Config:
    """
    Centralised configuration class.

    Every attribute maps 1-to-1 to an environment variable defined in .env.
    Call Config.validate() at application startup to fail-fast if any
    required variable is absent.
    """

    # ------------------------------------------------------------------
    # Groq AI (FREE — 6,000 requests/day, no credit card)
    # ------------------------------------------------------------------
    GROQ_API_KEY: str = os.getenv("GROQ_API_KEY", "")

    # ------------------------------------------------------------------
    # Telegram
    # ------------------------------------------------------------------
    TELEGRAM_BOT_TOKEN: str = os.getenv("TELEGRAM_BOT_TOKEN", "").strip().replace("%0A", "")
    TELEGRAM_CHAT_ID: str = os.getenv("TELEGRAM_CHAT_ID", "").strip().replace("%0A", "")

    # ------------------------------------------------------------------
    # LeetCode
    # ------------------------------------------------------------------
    LEETCODE_SESSION: str = os.getenv("LEETCODE_SESSION", "")
    LEETCODE_CSRF_TOKEN: str = os.getenv("LEETCODE_CSRF_TOKEN", "")
    LEETCODE_2_SESSION: str = os.getenv("LEETCODE_2_SESSION", "")
    LEETCODE_2_CSRF_TOKEN: str = os.getenv("LEETCODE_2_CSRF_TOKEN", "")


    # ------------------------------------------------------------------
    # Codeforces
    # ------------------------------------------------------------------
    CODEFORCES_API_KEY: str = os.getenv("CODEFORCES_API_KEY", "")
    CODEFORCES_API_SECRET: str = os.getenv("CODEFORCES_API_SECRET", "")
    CODEFORCES_HANDLE: str = os.getenv("CODEFORCES_HANDLE", "")
    CODEFORCES_PASSWORD: str = os.getenv("CODEFORCES_PASSWORD", "")
    CODEFORCES_39CE7: str = os.getenv("CODEFORCES_39CE7", "")
    CODEFORCES_JSESSIONID: str = os.getenv("CODEFORCES_JSESSIONID", "")
    CODEFORCES_X_USER_SHA1: str = os.getenv("CODEFORCES_X_USER_SHA1", "")

    # ------------------------------------------------------------------
    # CodeChef — cookie-based auth + password login fallback
    # ------------------------------------------------------------------
    CODECHEF_USERNAME:     str = os.getenv("CODECHEF_USERNAME", "")
    CODECHEF_PASSWORD:     str = os.getenv("CODECHEF_PASSWORD", "")
    CODECHEF_AUTH_TOKEN:   str = os.getenv("CODECHEF_AUTH_TOKEN", "")
    CODECHEF_SESSION:      str = os.getenv("CODECHEF_SESSION", "")
    CODECHEF_CF_CLEARANCE: str = os.getenv("CODECHEF_CF_CLEARANCE", "")
    CODECHEF_UID:          str = os.getenv("CODECHEF_UID", "")
    CODECHEF_USERKEY:      str = os.getenv("CODECHEF_USERKEY", "")

    # ------------------------------------------------------------------
    # HackerRank
    # ------------------------------------------------------------------
    HACKERRANK_USERNAME: str = os.getenv("HACKERRANK_USERNAME", "")
    HACKERRANK_PASSWORD: str = os.getenv("HACKERRANK_PASSWORD", "")



    # ------------------------------------------------------------------
    # Bot behaviour settings
    # ------------------------------------------------------------------
    DAILY_RUN_TIME: str = os.getenv("DAILY_RUN_TIME", "08:00")
    SOLUTION_LANGUAGE: str = os.getenv("SOLUTION_LANGUAGE", "python3")
    LOG_LEVEL: str = os.getenv("LOG_LEVEL", "INFO")

    # ------------------------------------------------------------------
    # Internal constants (not from .env)
    # ------------------------------------------------------------------
    REQUEST_TIMEOUT: int = 30        # seconds for every outbound HTTP call
    RETRY_ATTEMPTS: int = 3          # number of retries for transient errors
    RETRY_DELAY: int = 5             # seconds between retries
    GROQ_MODEL: str = "openai/gpt-oss-120b"

    # ------------------------------------------------------------------
    # Required keys — the validate() method checks all of these.
    # ------------------------------------------------------------------
    _REQUIRED_KEYS: list = [
        "GROQ_API_KEY",
        "TELEGRAM_BOT_TOKEN",
        "TELEGRAM_CHAT_ID",
        # LEETCODE_SESSION and LEETCODE_CSRF_TOKEN are intentionally OPTIONAL here.
        # Browser session cookies expire frequently; their absence should only skip
        # LeetCode — not crash the entire bot before other platforms run.
        # Use has_leetcode_credentials() to check before the LeetCode pipeline.
        "CODEFORCES_API_KEY",
        "CODEFORCES_API_SECRET",
        "CODEFORCES_HANDLE",
        # CODEFORCES_PASSWORD is optional — only needed for browser-based submission,
        # not for problem fetching via the Codeforces API.
        "CODECHEF_AUTH_TOKEN",
        "CODECHEF_SESSION",
        "HACKERRANK_USERNAME",
        "HACKERRANK_PASSWORD",
    ]


    @classmethod
    def validate(cls) -> None:
        """
        Validate that every required environment variable has been set.

        Iterates over _REQUIRED_KEYS and checks that the corresponding class
        attribute is non-empty.  If one or more keys are missing, raises a
        ValueError listing every missing key so the developer can fix them
        all in one shot rather than discovering them one-by-one.

        Raises:
            ValueError: If one or more required environment variables are
                        missing or empty.  The message lists every missing key.

        Returns:
            None — if all keys are present the method returns silently.
        """
        missing: list[str] = []

        for key in cls._REQUIRED_KEYS:
            value = getattr(cls, key, None)
            if not value or value.strip() == "":
                missing.append(key)

        if missing:
            formatted = "\n  - ".join(missing)
            raise ValueError(
                f"The following required environment variables are missing or empty "
                f"in your .env file:\n  - {formatted}\n\n"
                f"Copy .env.example to .env and fill in the correct values."
            )

        logger.info("Config validation passed — all required keys are present.")
        if not cls.has_leetcode_credentials():
            logger.warning(
                "LEETCODE_SESSION / LEETCODE_CSRF_TOKEN are missing or empty. "
                "LeetCode will be skipped this run. "
                "Update the GitHub secret with fresh browser cookies to re-enable."
            )

    @classmethod
    def has_leetcode_credentials(cls, platform: str = "leetcode") -> bool:
        """
        Return True only when both LeetCode auth cookies for the specified
        account are non-empty.

        LeetCode session cookies expire frequently.  Callers should use this
        helper to decide whether to attempt the LeetCode pipeline rather than
        letting it fail deep inside the fetcher or submitter.

        Args:
            platform (str): The platform/account identifier. Can be 'leetcode' or 'leetcode_2'.

        Returns:
            bool: True if correct cookies are set.
        """
        if platform == "leetcode_2":
            return bool(
                cls.LEETCODE_2_SESSION and cls.LEETCODE_2_SESSION.strip()
                and cls.LEETCODE_2_CSRF_TOKEN and cls.LEETCODE_2_CSRF_TOKEN.strip()
            )
        return bool(
            cls.LEETCODE_SESSION and cls.LEETCODE_SESSION.strip()
            and cls.LEETCODE_CSRF_TOKEN and cls.LEETCODE_CSRF_TOKEN.strip()
        )


    @classmethod
    def get_run_hour_minute(cls) -> tuple[int, int]:
        """
        Parse DAILY_RUN_TIME into (hour, minute) integers.

        Expects the format "HH:MM" (24-hour clock).  Falls back to 08:00
        if the value cannot be parsed.

        Returns:
            tuple[int, int]: (hour, minute) both as integers.
        """
        try:
            parts = cls.DAILY_RUN_TIME.strip().split(":")
            hour = int(parts[0])
            minute = int(parts[1])
            if not (0 <= hour <= 23 and 0 <= minute <= 59):
                raise ValueError("Out of range")
            return hour, minute
        except (ValueError, IndexError):
            logger.warning(
                "DAILY_RUN_TIME '%s' is invalid — defaulting to 08:00.",
                cls.DAILY_RUN_TIME,
            )
            return 8, 0

    @classmethod
    def log_level_int(cls) -> int:
        """
        Convert the LOG_LEVEL string to a Python logging integer constant.

        Recognised values: DEBUG, INFO, WARNING, ERROR, CRITICAL.
        Defaults to logging.INFO for any unrecognised value.

        Returns:
            int: One of logging.DEBUG / INFO / WARNING / ERROR / CRITICAL.
        """
        level_map = {
            "DEBUG": logging.DEBUG,
            "INFO": logging.INFO,
            "WARNING": logging.WARNING,
            "ERROR": logging.ERROR,
            "CRITICAL": logging.CRITICAL,
        }
        level = cls.LOG_LEVEL.upper()
        if level not in level_map:
            logger.warning(
                "Unrecognised LOG_LEVEL '%s' — defaulting to INFO.", cls.LOG_LEVEL
            )
            return logging.INFO
        return level_map[level]
