import logging
from platforms.codechef import fetch_daily_problem

logging.basicConfig(level=logging.DEBUG)
try:
    problem = fetch_daily_problem()
    print("SUCCESS")
    print(problem)
except Exception as e:
    print("FAILED")
    print(e)
