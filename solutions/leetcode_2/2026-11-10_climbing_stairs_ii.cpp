#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n;
    if(!(cin >> n)) return 0;
    string line;
    vector<long long> costs;
    // read rest of line after n (might be newline)
    getline(cin, line);
    // read line containing array
    if(!getline(cin, line)) return 0;
    long long num = 0;
    bool inNumber = false, negative = false;
    for(char c : line) {
        if(c == '-' ) {
            negative = true;
        } else if(isdigit(c)) {
            num = num * 10 + (c - '0');
            inNumber = true;
        } else {
            if(inNumber) {
                if(negative) num = -num;
                costs.push_back(num);
                num = 0;
                inNumber = false;
                negative = false;
            }
        }
    }
    if(inNumber) {
        if(negative) num = -num;
        costs.push_back(num);
    }
    // ensure size matches n
    if((long long)costs.size() != n) {
        // maybe input without brackets, just numbers separated by spaces
        // try reading remaining numbers from cin
        while((long long)costs.size() < n && cin >> num) {
            costs.push_back(num);
        }
    }
    // make 1-indexed
    costs.insert(costs.begin(), 0); // now costs[1..n]
    
    const long long INF = (1LL<<60);
    vector<long long> dp(n+1, INF);
    dp[0] = 0;
    for(int i = 1; i <= n; ++i) {
        for(int k = 1; k <= 3; ++k) {
            if(i - k >= 0) {
                long long cand = dp[i-k] + costs[i] + 1LL*k*k;
                if(cand < dp[i]) dp[i] = cand;
            }
        }
    }
    cout << dp[n];
    return 0;
}