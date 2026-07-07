"""
scheduler.py — APScheduler-based daily runner for the Daily Coding Bot.

Schedules run_daily_bot() from main.py to execute automatically every day
at the time configured in the DAILY_RUN_TIME environment variable.

Start the bot with:
    python scheduler.py

The process will stay alive indefinitely, printing next-run info on startup
and waking up at the configured time to execute the pipeline.  Press
Ctrl+C to stop cleanly.
"""

import asyncio
import logging
import sys
import time
from datetime import datetime
from pathlib import Path

from apscheduler.schedulers.background import BackgroundScheduler
from apscheduler.triggers.cron import CronTrigger

from config import Config
from main import run_daily_bot, _configure_logging

logger = logging.getLogger(__name__)


# ---------------------------------------------------------------------------
# Job wrapper
# ---------------------------------------------------------------------------

def _job_wrapper() -> None:
    """
    Synchronous wrapper that APScheduler calls on its background thread.

    Creates a new asyncio event loop for the job execution, runs the
    async run_daily_bot() coroutine to completion, then closes the loop.
    This pattern is necessary because APScheduler's BackgroundScheduler
    operates on a regular thread pool that has no existing event loop.

    If run_daily_bot() raises an unhandled exception, it is caught here,
    logged with full traceback, and does NOT propagate — so APScheduler
    continues scheduling future runs.
    """
    logger.info("APScheduler triggered — starting daily bot run…")
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    try:
        loop.run_until_complete(run_daily_bot())
        logger.info("Daily bot run completed successfully.")
    except Exception as exc:
        logger.error(
            "Unhandled exception in daily bot run: %s", exc, exc_info=True
        )
    finally:
        loop.close()
        asyncio.set_event_loop(None)


# ---------------------------------------------------------------------------
# Scheduler setup
# ---------------------------------------------------------------------------

def start_scheduler() -> None:
    """
    Initialise and start the APScheduler BackgroundScheduler.

    Reads the target run time from Config.get_run_hour_minute() and creates
    a daily CronTrigger.  Prints the next scheduled execution time to the
    console and keeps the main thread alive with a polling loop.

    Handles KeyboardInterrupt (Ctrl+C) cleanly by shutting the scheduler
    down and exiting with a friendly message.

    Raises:
        ValueError: If Config.validate() fails because required .env keys
                    are missing — the scheduler will not start in this case.
    """
    _configure_logging()

    logger.info("Validating configuration before starting scheduler…")
    Config.validate()

    hour, minute = Config.get_run_hour_minute()

    scheduler = BackgroundScheduler(
        job_defaults={
            "coalesce": True,       # collapse multiple missed fires into one
            "max_instances": 1,     # never run two instances at the same time
            "misfire_grace_time": 300,  # allow up to 5 min late start
        }
    )

    scheduler.add_job(
        func=_job_wrapper,
        trigger=CronTrigger(hour=hour, minute=minute),
        id="daily_coding_bot",
        name="Daily Coding Challenge Bot",
    )

    scheduler.start()

    # Determine and display next scheduled run time
    job = scheduler.get_job("daily_coding_bot")
    next_run: datetime = job.next_run_time if job else None  # type: ignore[assignment]
    next_run_str = next_run.strftime("%Y-%m-%d %H:%M:%S %Z") if next_run else "Unknown"

    print("╔══════════════════════════════════════════════╗")
    print("║       Daily Coding Challenge Bot             ║")
    print("╠══════════════════════════════════════════════╣")
    print(f"║  Scheduled daily at:  {hour:02d}:{minute:02d}                   ║")
    print(f"║  Next execution:      {next_run_str:<22} ║")
    print("║  Press Ctrl+C to stop.                       ║")
    print("╚══════════════════════════════════════════════╝")

    logger.info(
        "Scheduler started — daily job at %02d:%02d. Next run: %s",
        hour,
        minute,
        next_run_str,
    )

    try:
        while True:
            time.sleep(60)
    except KeyboardInterrupt:
        logger.info("KeyboardInterrupt received — shutting down scheduler…")
        scheduler.shutdown(wait=False)
        print("\nBot stopped. Goodbye!")
        sys.exit(0)


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    start_scheduler()
