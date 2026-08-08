import logging
import sys
logging.basicConfig(level=logging.INFO, stream=sys.stdout)

from platforms.submitters.hackerrank_submit import submit_solution

problem = {"url": "https://www.hackerrank.com/challenges/solve-me-first/problem"}
code = """
#include <iostream>
using namespace std;
int solveMeFirst(int a, int b) {
 return a+b;
}
int main() {
  int num1, num2;
  int sum;
  cin>>num1>>num2;
  sum = solveMeFirst(num1,num2);
  cout<<sum;
  return 0;
}
"""
try:
    res = submit_solution(problem, code)
    print("RESULT:", res)
except Exception as e:
    print("ERROR:", e)
