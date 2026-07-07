"""
storage/db.py — SQLite persistence layer for the Daily Coding Challenge Bot.

Provides a lightweight, dependency-free database interface using Python's
built-in sqlite3 module.  All functions operate on a single table
(solved_problems) that records every problem the bot solves.

The database file is created automatically at first run inside the
project root as  daily_bot.db.

Usage:
    from storage.db import init_db, log_solution, get_today_solved, get_all_solved
    init_db()
    log_solution("leetcode", "Two Sum", "https://...", "python3", "solutions/...")
"""

import sqlite3
import logging
import os
from datetime import date, datetime
from typing import Optional

logger = logging.getLogger(__name__)

# Path to the SQLite database file — placed in the project root.
_DB_PATH: str = os.path.join(os.path.dirname(os.path.dirname(__file__)), "daily_bot.db")

# DDL executed once on startup.
_CREATE_TABLE_SQL: str = """
CREATE TABLE IF NOT EXISTS solved_problems (
    id                 INTEGER PRIMARY KEY AUTOINCREMENT,
    platform           TEXT      NOT NULL,
    problem_title      TEXT      NOT NULL,
    problem_url        TEXT,
    solution_language  TEXT      NOT NULL,
    solution_file_path TEXT      NOT NULL,
    solved_at          TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    submission_status  TEXT      DEFAULT 'saved',
    submission_id      TEXT
);
"""


def _get_connection() -> sqlite3.Connection:
    """
    Open and return a new SQLite connection with row_factory set to
    sqlite3.Row so callers can access columns by name as well as index.

    Returns:
        sqlite3.Connection: An open connection to the local database file.

    Raises:
        sqlite3.Error: If the database file cannot be opened or created.
    """
    conn = sqlite3.connect(_DB_PATH, detect_types=sqlite3.PARSE_DECLTYPES)
    conn.row_factory = sqlite3.Row
    conn.execute("PRAGMA journal_mode=WAL;")   # safer concurrent writes
    return conn


def init_db() -> None:
    """
    Initialise the database by creating the solved_problems table if it does
    not already exist.

    This function is idempotent — calling it multiple times is safe.  It
    should be called once at bot startup before any other db functions are
    used.

    Raises:
        sqlite3.Error: If the table cannot be created (e.g. permission error).
    """
    logger.info("Initialising database at %s", _DB_PATH)
    with _get_connection() as conn:
        conn.execute(_CREATE_TABLE_SQL)
        try:
            conn.execute("ALTER TABLE solved_problems ADD COLUMN submission_id TEXT;")
        except sqlite3.OperationalError:
            pass  # Column already exists
        conn.commit()
    logger.info("Database initialised successfully.")


def log_solution(
    platform: str,
    title: str,
    url: Optional[str],
    language: str,
    file_path: str,
    status: str = "saved",
    submission_id: Optional[str] = None,
) -> int:
    """
    Insert a new row into solved_problems to record a solved problem.

    Args:
        platform      (str): One of 'leetcode', 'codeforces', 'codechef',
                             'hackerrank'.
        title         (str): Human-readable problem title (e.g. "Two Sum").
        url           (str | None): Direct URL to the problem; may be None if
                             unavailable.
        language      (str): Programming language of the solution (e.g. 'python3').
        file_path     (str): Relative or absolute path to the saved solution file.
        status        (str): Submission status string; defaults to 'saved'.
        submission_id (str | None): Platform's submission ID.

    Returns:
        int: The auto-generated row ID (lastrowid) of the inserted record.

    Raises:
        sqlite3.Error: If the INSERT statement fails.
    """
    sql = """
        INSERT INTO solved_problems
            (platform, problem_title, problem_url, solution_language,
             solution_file_path, submission_status, submission_id)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    """
    with _get_connection() as conn:
        cursor = conn.execute(sql, (platform, title, url, language, file_path, status, submission_id))
        conn.commit()
        row_id: int = cursor.lastrowid  # type: ignore[assignment]

    logger.info(
        "Logged solution — platform=%s  title='%s'  row_id=%d",
        platform,
        title,
        row_id,
    )
    return row_id


def get_today_solved(platform: str) -> bool:
    """
    Check whether a problem from the given platform has already been solved
    and recorded today (based on the local calendar date).

    This prevents the bot from fetching and solving the same daily problem
    twice if it is restarted or encounters a transient error midway.

    Args:
        platform (str): The platform name to check (e.g. 'leetcode').

    Returns:
        bool: True if at least one row for this platform exists with a
              solved_at timestamp on today's date; False otherwise.

    Raises:
        sqlite3.Error: If the SELECT query fails.
    """
    today_str: str = date.today().isoformat()   # e.g. "2024-01-15"
    sql = """
        SELECT COUNT(*) AS cnt
        FROM   solved_problems
        WHERE  platform  = ?
          AND  DATE(solved_at) = ?
    """
    with _get_connection() as conn:
        row = conn.execute(sql, (platform, today_str)).fetchone()
        count: int = row["cnt"] if row else 0

    already_done = count > 0
    logger.debug(
        "get_today_solved(platform=%s, date=%s) → %s",
        platform,
        today_str,
        already_done,
    )
    return already_done


def get_all_solved() -> list[dict]:
    """
    Retrieve every row from solved_problems ordered from newest to oldest.

    Returns:
        list[dict]: A list of dicts, one per row.  Each dict contains the
                    following keys:
                        id, platform, problem_title, problem_url,
                        solution_language, solution_file_path,
                        solved_at, submission_status

    Raises:
        sqlite3.Error: If the SELECT query fails.
    """
    sql = """
        SELECT id, platform, problem_title, problem_url,
               solution_language, solution_file_path,
               solved_at, submission_status, submission_id
        FROM   solved_problems
        ORDER  BY solved_at DESC
    """
    with _get_connection() as conn:
        rows = conn.execute(sql).fetchall()

    results: list[dict] = [dict(row) for row in rows]
    logger.debug("get_all_solved() returned %d rows.", len(results))
    return results


def get_platform_stats() -> list[dict]:
    """
    Return a per-platform summary of how many problems have been solved.

    Useful for the README or a Telegram summary message.

    Returns:
        list[dict]: Each dict has keys 'platform' and 'total_solved'.
                    Ordered by total_solved descending.

    Raises:
        sqlite3.Error: If the aggregation query fails.
    """
    sql = """
        SELECT   platform,
                 COUNT(*) AS total_solved
        FROM     solved_problems
        GROUP BY platform
        ORDER BY total_solved DESC
    """
    with _get_connection() as conn:
        rows = conn.execute(sql).fetchall()

    results: list[dict] = [dict(row) for row in rows]
    logger.debug("get_platform_stats() → %s", results)
    return results
