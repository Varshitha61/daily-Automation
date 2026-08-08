import os
import sys

# Ensure the project root is in python path
sys.path.append(os.path.dirname(os.path.abspath(__file__)))

from config import Config
from platforms.submitters.codeforces_submit import submit_solution

def main():
    print("Testing Codeforces Direct Cookie Submission...")
    print("-" * 50)
    print(f"CODEFORCES_HANDLE:     {Config.CODEFORCES_HANDLE}")
    print(f"CODEFORCES_39CE7:      {Config.CODEFORCES_39CE7[:10]}... (len={len(Config.CODEFORCES_39CE7)})" if Config.CODEFORCES_39CE7 else "MISSING")
    print(f"CODEFORCES_JSESSIONID: {Config.CODEFORCES_JSESSIONID[:10]}... (len={len(Config.CODEFORCES_JSESSIONID)})" if Config.CODEFORCES_JSESSIONID else "MISSING")
    print(f"CODEFORCES_X_USER_SHA1: {Config.CODEFORCES_X_USER_SHA1[:10]}... (len={len(Config.CODEFORCES_X_USER_SHA1)})" if Config.CODEFORCES_X_USER_SHA1 else "MISSING")
    print("-" * 50)

    if not Config.CODEFORCES_39CE7:
        print("ERROR: Please configure CODEFORCES_39CE7 in your .env file first.")
        return

    # We will submit a dummy C++ print solution to a well-known easy problem (e.g. 4A - Watermelon)
    problem = {
        "title": "Watermelon",
        "contest_id": 4,
        "index": "A"
    }

    # Dummy C++ solution that prints NO (an incorrect solution is safe to test compilation and submissions)
    code = """#include <iostream>
using namespace std;
int main() {
    int w;
    cin >> w;
    if (w % 2 == 0 && w > 2) {
        cout << "YES" << endl;
    } else {
        cout << "NO" << endl;
    }
    return 0;
}
"""

    print("Attempting dry-run submission to Codeforces problem 4A (Watermelon)...")
    try:
        result = submit_solution(problem, code)
        print("\nSubmission results:")
        print("-" * 50)
        print(f"Verdict:       {result.get('verdict')}")
        print(f"Accepted:      {result.get('accepted')}")
        print(f"Submission ID: {result.get('submission_id')}")
        print(f"URL:           {result.get('url')}")
        print(f"Runtime:       {result.get('runtime')}")
        print(f"Memory:        {result.get('memory')}")
        print("-" * 50)
    except Exception as e:
        print("\nSubmission failed with error:")
        print(e)

if __name__ == "__main__":
    main()
