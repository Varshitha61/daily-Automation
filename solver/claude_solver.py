"""
solver/claude_solver.py — AI solution generator using the Anthropic Claude API.

Sends a structured prompt to Claude claude-sonnet-4-6 containing the full problem
description and requests a raw, executable solution in the configured
programming language.

Credentials required in .env:
    ANTHROPIC_API_KEY — from https://console.anthropic.com/account/keys

Usage:
    from solver.claude_solver import solve_problem
    result = solve_problem(problem_dict)
    # result → { code, language, problem_title, platform }
"""

import asyncio
import logging
import time
from typing import Optional

import anthropic

from config import Config

logger = logging.getLogger(__name__)

# Maximum number of tokens to request in a single Claude response.
# 4096 is generous for most competitive-programming solutions.
_MAX_TOKENS: int = 4096

# Seconds to wait before retrying a failed Claude call.
_RETRY_WAIT: int = 10


# ---------------------------------------------------------------------------
# Prompt builders
# ---------------------------------------------------------------------------

def _build_system_prompt(language: str) -> str:
    """
    Build the system prompt that instructs Claude to act as a competitive
    programmer and return only raw, executable code.

    Args:
        language (str): Target programming language (e.g. 'python3', 'cpp').

    Returns:
        str: The complete system prompt string.
    """
    return (
        f"You are an expert competitive programmer. "
        f"Solve the given problem with the most efficient algorithm possible. "
        f"Return ONLY the solution code in {language}. "
        f"No explanation. No markdown. No backticks. "
        f"Just raw executable code."
    )


def _build_user_message(problem: dict) -> str:
    """
    Build the user-turn message containing all problem context Claude needs.

    Includes the platform name, problem title, full description, and any
    example test cases that are available in the problem dict.

    Args:
        problem (dict): A problem dict as returned by any platform fetcher.
                        Expected keys: title, description, platform.
                        Optional keys: difficulty, examples, rating.

    Returns:
        str: A formatted multi-line string ready to send as the user message.
    """
    platform: str = problem.get("platform", "Unknown Platform").upper()
    title: str = problem.get("title", "Unknown Problem")
    description: str = problem.get("description", "No description provided.")
    difficulty: str = problem.get("difficulty", problem.get("rating", "Unknown"))

    lines: list[str] = [
        f"Platform: {platform}",
        f"Problem Title: {title}",
        f"Difficulty: {difficulty}",
        "",
        "=== Problem Description ===",
        description,
    ]

    # Include example test cases if separately available (LeetCode provides them)
    examples: str = problem.get("examples", "")
    if examples and examples not in description:
        lines.extend(["", "=== Example Test Cases ===", examples])

    return "\n".join(lines)


# ---------------------------------------------------------------------------
# Core solver
# ---------------------------------------------------------------------------

def solve_problem(problem: dict) -> dict:
    """
    Generate a complete code solution for the given problem using Claude.

    Builds a structured prompt from the problem dict, calls the Anthropic
    claude-sonnet-4-6 model, and returns the raw solution code along with
    metadata.  If the first API call fails, it retries once after
    _RETRY_WAIT seconds.  If both attempts fail, the error is logged and
    re-raised so the caller (main.py) can handle it gracefully.

    Args:
        problem (dict): A problem dictionary as returned by any platform
                        fetcher.  Required keys:
                            - title       (str): Human-readable problem name
                            - description (str): Full problem statement
                            - platform    (str): Platform identifier
                        Optional keys (included in the prompt when present):
                            - difficulty / rating, examples

    Returns:
        dict: A solution dictionary with the following keys:
            - code          (str): Raw, executable solution code with no
                                   markdown formatting or surrounding text.
            - language      (str): Programming language (from Config).
            - problem_title (str): Title of the problem that was solved.
            - platform      (str): Platform the problem came from.

    Raises:
        RuntimeError: If both Claude API call attempts fail.  The caller
                      should catch this and send a Telegram error notification
                      rather than crashing the bot.
    """
    language: str = Config.SOLUTION_LANGUAGE
    platform: str = problem.get("platform", "unknown")
    title: str = problem.get("title", "Unknown Problem")

    logger.info(
        "Solving '%s' [%s] in %s via Claude %s…",
        title,
        platform,
        language,
        Config.CLAUDE_MODEL,
    )

    system_prompt: str = _build_system_prompt(language)
    user_message: str = _build_user_message(problem)

    client = anthropic.Anthropic(api_key=Config.ANTHROPIC_API_KEY)

    last_exc: Optional[Exception] = None

    for attempt in range(1, 3):  # Max 2 attempts
        try:
            logger.debug("Claude API call — attempt %d/2", attempt)

            message = client.messages.create(
                model=Config.CLAUDE_MODEL,
                max_tokens=_MAX_TOKENS,
                system=system_prompt,
                messages=[
                    {"role": "user", "content": user_message}
                ],
            )

            # Extract the text from the first content block
            if not message.content:
                raise RuntimeError("Claude returned an empty response (no content blocks).")

            raw_code: str = message.content[0].text.strip()

            if not raw_code:
                raise RuntimeError("Claude returned a blank code string.")

            logger.info(
                "Claude solved '%s' — %d characters of %s code generated.",
                title,
                len(raw_code),
                language,
            )

            return {
                "code": raw_code,
                "language": language,
                "problem_title": title,
                "platform": platform,
            }

        except anthropic.APIConnectionError as exc:
            last_exc = exc
            logger.warning(
                "Claude API connection error on attempt %d: %s", attempt, exc
            )
        except anthropic.RateLimitError as exc:
            last_exc = exc
            logger.warning(
                "Claude API rate limit hit on attempt %d: %s", attempt, exc
            )
        except anthropic.APIStatusError as exc:
            last_exc = exc
            logger.warning(
                "Claude API status error %d on attempt %d: %s",
                exc.status_code,
                attempt,
                exc.message,
            )
        except RuntimeError as exc:
            last_exc = exc
            logger.warning(
                "Claude response parsing error on attempt %d: %s", attempt, exc
            )

        if attempt < 2:
            logger.info(
                "Retrying Claude after %d seconds…", _RETRY_WAIT
            )
            time.sleep(_RETRY_WAIT)

    raise RuntimeError(
        f"Claude failed to solve '{title}' [{platform}] after 2 attempts. "
        f"Last error: {last_exc}"
    )
