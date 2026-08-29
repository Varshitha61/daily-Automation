import logging
from platforms.submitters.hackerrank_submit import submit_solution

logging.basicConfig(level=logging.INFO)

problem = {
    'title': 'Compare the Triplets',
    'url': 'https://www.hackerrank.com/challenges/compare-the-triplets/problem?isFullScreen=true'
}

code = """#include <bits/stdc++.h>
using namespace std;
vector<int> compareTriplets(vector<int> a, vector<int> b) {
    int alice = 0, bob = 0;
    for(int i=0; i<3; i++){
        if(a[i] > b[i]) alice++;
        else if(a[i] < b[i]) bob++;
    }
    return {alice, bob};
}
int main() {
    vector<int> a(3), b(3);
    for(int i=0; i<3; i++) cin >> a[i];
    for(int i=0; i<3; i++) cin >> b[i];
    vector<int> res = compareTriplets(a, b);
    cout << res[0] << " " << res[1] << endl;
    return 0;
}"""

print('Testing HackerRank submit...')
result = submit_solution(problem, code)
print('Result:', result)
