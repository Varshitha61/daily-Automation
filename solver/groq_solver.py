"""
solver/groq_solver.py — AI solution generator using Groq API (FREE tier).

Uses the Groq SDK to call Qwen3.8-27B — completely free with 6,000
requests/day. No projects or billing needed.

Free API key at: https://console.groq.com/keys

Credentials required in .env:
    GROQ_API_KEY — from https://console.groq.com/keys

Usage:
    from solver.groq_solver import solve_problem
    result = solve_problem(problem_dict)
    # result → { code, language, problem_title, platform }
"""

import logging
import time
from typing import Optional

from groq import Groq

from config import Config

logger = logging.getLogger(__name__)

# openai/gpt-oss-120b is the largest model available on Groq — best for competitive programming.
# Updated from qwen/qwen3.8-27b which had compile errors on complex C++ problems.
_MODEL_NAME: str = "openai/gpt-oss-120b"

# Seconds to wait before retrying a failed call.
_RETRY_WAIT: int = 10


# ---------------------------------------------------------------------------
# Prompt builder
# ---------------------------------------------------------------------------

def _build_system_prompt(language: str) -> str:
    return (
        f"You are an expert competitive programmer. "
        f"Solve the given problem with the most efficient algorithm possible. "
        f"Return ONLY the solution code in {language}. "
        f"No explanation. No markdown. No backticks. Just raw executable code."
    )


def _build_user_message(problem: dict) -> str:
    platform: str    = problem.get("platform", "Unknown Platform").upper()
    title: str       = problem.get("title", "Unknown Problem")
    description: str = problem.get("description", "No description provided.")
    difficulty: str  = problem.get("difficulty", problem.get("rating", "Unknown"))

    lines: list[str] = [
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
    Generate a complete code solution using Groq (gpt-oss-120b) — free tier.

    Args:
        problem (dict): Problem dictionary from any platform fetcher.
                        Required keys: title, description, platform.

    Returns:
        dict: { code, language, problem_title, platform }

    Raises:
        RuntimeError: If both API call attempts fail.
    """
    language: str = Config.SOLUTION_LANGUAGE
    platform: str = problem.get("platform", "unknown")
    title: str    = problem.get("title", "Unknown Problem")

    logger.info(
        "Solving '%s' [%s] in %s via Groq (%s)...",
        title, platform, language, _MODEL_NAME,
    )

    client = Groq(api_key=Config.GROQ_API_KEY)
    system_prompt = _build_system_prompt(language)
    user_message  = _build_user_message(problem)
    last_exc: Optional[Exception] = None

    for attempt in range(1, 3):  # Max 2 attempts
        try:
            logger.debug("Groq API call — attempt %d/2", attempt)

            completion = client.chat.completions.create(
                model=_MODEL_NAME,
                messages=[
                    {"role": "system", "content": system_prompt},
                    {"role": "user",   "content": user_message},
                ],
                max_tokens=4096,
                temperature=0.2,
            )

            raw_code: str = completion.choices[0].message.content or ""
            raw_code = raw_code.strip()

            # Strip accidental markdown fences (```cpp ... ```)
            if raw_code.startswith("```"):
                lines = raw_code.splitlines()
                inner = lines[1:] if lines[0].startswith("```") else lines
                if inner and inner[-1].strip() == "```":
                    inner = inner[:-1]
                raw_code = "\n".join(inner).strip()

            if not raw_code:
                raise RuntimeError("Groq returned a blank code string.")

            logger.info(
                "Groq solved '%s' — %d characters of %s code generated.",
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
            logger.warning("Groq API error on attempt %d: %s", attempt, exc)

        if attempt < 2:
            logger.info("Retrying Groq after %d seconds...", _RETRY_WAIT)
            time.sleep(_RETRY_WAIT)

    raise RuntimeError(
        f"Groq failed to solve '{title}' [{platform}] after 2 attempts. "
        f"Last error: {last_exc}"
    )
