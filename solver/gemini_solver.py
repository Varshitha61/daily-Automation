"""
solver/gemini_solver.py — AI solution generator using Google Gemini API (FREE tier).

Uses the new google-genai SDK to call gemini-1.5-flash, which has a generous
free tier of 1,500 requests/day — more than enough for a daily bot.

Free API key at: https://aistudio.google.com/app/apikey

Credentials required in .env:
    GEMINI_API_KEY — from https://aistudio.google.com/app/apikey

Usage:
    from solver.gemini_solver import solve_problem
    result = solve_problem(problem_dict)
    # result → { code, language, problem_title, platform }
"""

import logging
import time
from typing import Optional

from google import genai
from google.genai import types

from config import Config

logger = logging.getLogger(__name__)

# gemini-2.0-flash-lite is free, fast, and available on this project.
_MODEL_NAME: str = "models/gemini-2.0-flash-lite"

# Seconds to wait before retrying a failed call.
_RETRY_WAIT: int = 15


# ---------------------------------------------------------------------------
# Prompt builder
# ---------------------------------------------------------------------------

def _build_prompt(problem: dict, language: str) -> str:
    """Build a combined prompt for Gemini."""
    platform: str    = problem.get("platform", "Unknown Platform").upper()
    title: str       = problem.get("title", "Unknown Problem")
    description: str = problem.get("description", "No description provided.")
    difficulty: str  = problem.get("difficulty", problem.get("rating", "Unknown"))

    lines: list[str] = [
        f"You are an expert competitive programmer.",
        f"Solve the following problem with the most efficient algorithm possible.",
        f"Return ONLY the solution code in {language}.",
        f"No explanation. No markdown. No backticks. Just raw executable code.",
        "",
        f"Platform: {platform}",
        f"Problem Title: {title}",
        f"Difficulty: {difficulty}",
        "",
        "=== Problem Description ===",
        description,
    ]

    examples: str = problem.get("examples", "")
    if examples and examples not in description:
        lines.extend(["", "=== Example Test Cases ===", examples])

    return "\n".join(lines)


# ---------------------------------------------------------------------------
# Core solver
# ---------------------------------------------------------------------------

def solve_problem(problem: dict) -> dict:
    """
    Generate a complete code solution using Google Gemini 1.5 Flash (free tier).

    Args:
        problem (dict): A problem dictionary from any platform fetcher.
                        Required keys: title, description, platform.

    Returns:
        dict: { code, language, problem_title, platform }

    Raises:
        RuntimeError: If both Gemini API call attempts fail.
    """
    language: str = Config.SOLUTION_LANGUAGE
    platform: str = problem.get("platform", "unknown")
    title: str    = problem.get("title", "Unknown Problem")

    logger.info(
        "Solving '%s' [%s] in %s via Gemini (%s)...",
        title, platform, language, _MODEL_NAME,
    )

    client = genai.Client(api_key=Config.GEMINI_API_KEY)
    prompt: str = _build_prompt(problem, language)
    last_exc: Optional[Exception] = None

    for attempt in range(1, 3):  # Max 2 attempts
        try:
            logger.debug("Gemini API call — attempt %d/2", attempt)

            response = client.models.generate_content(
                model=_MODEL_NAME,
                contents=prompt,
                config=types.GenerateContentConfig(
                    max_output_tokens=4096,
                    temperature=0.2,
                ),
            )

            raw_code: str = response.text.strip() if response.text else ""

            # Strip accidental markdown fences (```cpp ... ```)
            if raw_code.startswith("```"):
                lines = raw_code.splitlines()
                inner = lines[1:] if lines[0].startswith("```") else lines
                if inner and inner[-1].strip() == "```":
                    inner = inner[:-1]
                raw_code = "\n".join(inner).strip()

            if not raw_code:
                raise RuntimeError("Gemini returned a blank code string.")

            logger.info(
                "Gemini solved '%s' — %d characters of %s code generated.",
                title, len(raw_code), language,
            )

            return {
                "code": raw_code,
                "language": language,
                "problem_title": title,
                "platform": platform,
            }

        except Exception as exc:
            last_exc = exc
            logger.warning("Gemini API error on attempt %d: %s", attempt, exc)

        if attempt < 2:
            logger.info("Retrying Gemini after %d seconds...", _RETRY_WAIT)
            time.sleep(_RETRY_WAIT)

    raise RuntimeError(
        f"Gemini failed to solve '{title}' [{platform}] after 2 attempts. "
        f"Last error: {last_exc}"
    )
