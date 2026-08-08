import os
import sys

# Ensure the project root is in python path
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from config import Config
from platforms.atcoder import fetch_daily_problem

def main():
    print("Validating configuration...")
    try:
        Config.validate()
        print("Configuration validation passed.")
    except Exception as e:
        print("Configuration validation failed:", e)
        print("Note: If AtCoder credentials are not filled, this is expected to fail.")
        # We can still try to run the fetcher, as fetching AtCoder problems doesn't require login.

    print("\nAttempting to fetch AtCoder daily problem...")
    try:
        problem = fetch_daily_problem()
        print("\nSuccessfully fetched problem:")
        print("-" * 40)
        print(f"Title:       {problem.get('title')}")
        print(f"URL:         {problem.get('url')}")
        print(f"Difficulty:  {problem.get('difficulty')}")
        print(f"Contest ID:  {problem.get('contest_id')}")
        print(f"Problem ID:  {problem.get('problem_id')}")
        print(f"Task Char:   {problem.get('task_char')}")
        print(f"Description length: {len(problem.get('description', ''))} chars")
        print("-" * 40)
        print("Sample of Description:")
        desc_sample = problem.get('description', '')[:500] + "..."
        safe_sample = desc_sample.encode(sys.stdout.encoding or 'utf-8', errors='replace').decode(sys.stdout.encoding or 'utf-8')
        print(safe_sample)
        print("-" * 40)
    except Exception as e:
        print("Failed to fetch AtCoder daily problem:", e)

if __name__ == "__main__":
    main()
