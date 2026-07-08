"""
platforms/leetcode.py — LeetCode daily problem fetcher.

Uses LeetCode's public GraphQL API endpoint with session-cookie
authentication to fetch the active daily coding challenge question and
its full problem description.

Credentials required in .env:
    LEETCODE_SESSION    — value of the LEETCODE_SESSION browser cookie
    LEETCODE_CSRF_TOKEN — value of the csrftoken browser cookie

Usage:
    from platforms.leetcode import fetch_daily_problem
    problem = fetch_daily_problem()
    # problem → { title, slug, difficulty, description, url, platform }
"""

import logging
import time
import re
import html
from typing import Optional

import requests

from config import Config

logger = logging.getLogger(__name__)

_GRAPHQL_URL: str = "https://leetcode.com/graphql"
_PROBLEM_BASE_URL: str = "https://leetcode.com/problems"

# ---------------------------------------------------------------------------
# GraphQL query bodies
# ---------------------------------------------------------------------------

_DAILY_CHALLENGE_QUERY: str = """
query getDailyChallenge {
  activeDailyCodingChallengeQuestion {
    date
    link
    question {
      questionId
      title
      titleSlug
      difficulty
      content
      exampleTestcases
      topicTags {
        name
      }
    }
  }
}
"""

_PROBLEM_DETAIL_QUERY: str = """
query getProblemDetail($titleSlug: String!) {
  question(titleSlug: $titleSlug) {
    questionId
    title
    titleSlug
    difficulty
    content
    exampleTestcases
    hints
  }
}
"""


def _build_session() -> requests.Session:
    """
    Build and return a requests.Session pre-configured with the LeetCode
    authentication cookies and required headers.

    The LEETCODE_SESSION and LEETCODE_CSRF_TOKEN values are read from Config
    and injected as cookies and the X-CSRFToken header respectively.

    Returns:
        requests.Session: A fully configured session ready to call the
                          LeetCode GraphQL endpoint.
    """
    session = requests.Session()
    session.cookies.set("LEETCODE_SESSION", Config.LEETCODE_SESSION, domain="leetcode.com")
    session.cookies.set("csrftoken", Config.LEETCODE_CSRF_TOKEN, domain="leetcode.com")
    session.headers.update(
        {
            "Content-Type": "application/json",
            "Referer": "https://leetcode.com",
            "X-CSRFToken": Config.LEETCODE_CSRF_TOKEN,
            "User-Agent": (
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                "AppleWebKit/537.36 (KHTML, like Gecko) "
                "Chrome/124.0.0.0 Safari/537.36"
            ),
        }
    )
    return session


def _graphql_post(
    session: requests.Session,
    query: str,
    variables: Optional[dict] = None,
) -> dict:
    """
    Send a POST request to the LeetCode GraphQL endpoint with automatic
    retries on transient HTTP errors.

    Retries up to Config.RETRY_ATTEMPTS times, waiting Config.RETRY_DELAY
    seconds between each attempt.

    Args:
        session   (requests.Session): The authenticated session to use.
        query     (str): The GraphQL query string.
        variables (dict | None): Optional GraphQL variables dict.

    Returns:
        dict: The parsed JSON response body.

    Raises:
        RuntimeError: If all retry attempts are exhausted without a successful
                      response, or if the response contains GraphQL errors.
    """
    payload: dict = {"query": query}
    if variables:
        payload["variables"] = variables

    last_exception: Optional[Exception] = None

    for attempt in range(1, Config.RETRY_ATTEMPTS + 1):
        try:
            logger.debug(
                "LeetCode GraphQL POST — attempt %d/%d", attempt, Config.RETRY_ATTEMPTS
            )
            response = session.post(
                _GRAPHQL_URL,
                json=payload,
                timeout=Config.REQUEST_TIMEOUT,
            )

            # Detect expired / invalid session cookies early
            if response.status_code in (401, 403):
                raise RuntimeError(
                    f"LeetCode returned HTTP {response.status_code} — your "
                    "LEETCODE_SESSION / LEETCODE_CSRF_TOKEN cookies have most likely "
                    "expired. Log into leetcode.com in your browser, copy the fresh "
                    "cookies, and update the GitHub Actions secrets."
                )

            response.raise_for_status()
            data = response.json()

            if "errors" in data:
                raise RuntimeError(
                    f"GraphQL errors returned: {data['errors']}"
                )

            return data

        except (requests.RequestException, RuntimeError) as exc:
            last_exception = exc
            logger.warning(
                "LeetCode request failed on attempt %d: %s", attempt, exc
            )
            if attempt < Config.RETRY_ATTEMPTS:
                logger.info("Retrying in %d seconds…", Config.RETRY_DELAY)
                time.sleep(Config.RETRY_DELAY)

    raise RuntimeError(
        f"LeetCode GraphQL endpoint failed after {Config.RETRY_ATTEMPTS} attempts. "
        f"Last error: {last_exception}"
    )


def _html_to_plain_text(html_content: str) -> str:
    """
    Convert an HTML problem description to readable plain text.

    Strips all HTML tags, decodes HTML entities, normalises whitespace, and
    removes the excessive blank lines that LeetCode embeds in its content.

    Args:
        html_content (str): Raw HTML string from the LeetCode API.

    Returns:
        str: Clean, human-readable plain text suitable for sending to Claude.
    """
    if not html_content:
        return ""

    # Replace common block-level tags with newlines before stripping
    for tag in ["</p>", "<br>", "<br/>", "<br />", "</li>", "</div>", "</pre>"]:
        html_content = html_content.replace(tag, "\n")

    # Strip all remaining HTML tags
    text = re.sub(r"<[^>]+>", "", html_content)

    # Decode HTML entities (e.g. &lt; → <, &amp; → &)
    text = html.unescape(text)

    # Collapse runs of blank lines to a single blank line
    text = re.sub(r"\n{3,}", "\n\n", text)

    return text.strip()


def fetch_daily_problem() -> dict:
    """
    Fetch today's LeetCode Daily Coding Challenge question via the GraphQL API.

    Authenticates using the session cookie and CSRF token from Config, queries
    the activeDailyCodingChallengeQuestion field, and returns a normalised
    problem dict.

    Returns:
        dict: A problem dictionary with the following keys:
            - title       (str)  : Problem title, e.g. "Two Sum"
            - slug        (str)  : URL-safe slug, e.g. "two-sum"
            - difficulty  (str)  : "Easy" | "Medium" | "Hard"
            - description (str)  : Full problem statement as plain text
            - url         (str)  : Direct URL to the problem on LeetCode
            - platform    (str)  : Always "leetcode"
            - question_id (str)  : Numeric question ID as a string
            - examples    (str)  : Raw example test cases string

    Raises:
        RuntimeError: If the API call fails or the expected fields are absent
                      in the response.
        KeyError:     If the response JSON structure is unexpected.
    """
    logger.info("Fetching LeetCode daily problem…")
    session = _build_session()
    data = _graphql_post(session, _DAILY_CHALLENGE_QUERY)

    try:
        challenge = data["data"]["activeDailyCodingChallengeQuestion"]
        question = challenge["question"]
    except (KeyError, TypeError) as exc:
        raise RuntimeError(
            f"Unexpected LeetCode API response structure: {exc}\n"
            f"Raw response keys: {list(data.get('data', {}).keys())}"
        ) from exc

    # challenge / question can be None when cookies are expired
    if challenge is None or question is None:
        raise RuntimeError(
            "LeetCode returned null for the daily challenge — your "
            "LEETCODE_SESSION / LEETCODE_CSRF_TOKEN cookies have most likely "
            "expired. Log into leetcode.com in your browser, copy the fresh "
            "cookies, and update the GitHub Actions secrets."
        )

    title: str = question["title"]
    slug: str = question["titleSlug"]
    difficulty: str = question["difficulty"]
    raw_content: str = question.get("content") or ""
    examples: str = question.get("exampleTestcases") or ""
    question_id: str = str(question.get("questionId", ""))
    problem_url: str = f"{_PROBLEM_BASE_URL}/{slug}/"

    description = _html_to_plain_text(raw_content)
    if examples:
        description += f"\n\nExample Test Cases:\n{examples}"

    logger.info(
        "LeetCode daily problem fetched — '%s' (%s) [id=%s]",
        title,
        difficulty,
        question_id,
    )

    return {
        "title": title,
        "slug": slug,
        "difficulty": difficulty,
        "description": description,
        "url": problem_url,
        "platform": "leetcode",
        "question_id": question_id,
        "examples": examples,
    }


def fetch_problem_details(slug: str) -> str:
    """
    Fetch the full problem content and example test cases for a specific
    LeetCode problem identified by its titleSlug.

    This is useful when you need the complete statement for a problem that
    is not necessarily today's daily challenge.

    Args:
        slug (str): The URL-safe problem slug, e.g. "two-sum".

    Returns:
        str: The full problem description as clean plain text, including
             example test cases appended at the end.

    Raises:
        RuntimeError: If the GraphQL call fails or the problem is not found.
    """
    logger.info("Fetching problem details for slug='%s'…", slug)
    session = _build_session()
    data = _graphql_post(
        session,
        _PROBLEM_DETAIL_QUERY,
        variables={"titleSlug": slug},
    )

    try:
        question = data["data"]["question"]
    except (KeyError, TypeError) as exc:
        raise RuntimeError(
            f"Problem '{slug}' not found or unexpected response: {exc}"
        ) from exc

    if question is None:
        raise RuntimeError(f"LeetCode returned null for problem slug '{slug}'.")

    raw_content: str = question.get("content") or ""
    examples: str = question.get("exampleTestcases") or ""

    description = _html_to_plain_text(raw_content)
    if examples:
        description += f"\n\nExample Test Cases:\n{examples}"

    logger.info("Problem details fetched for slug='%s'.", slug)
    return description
