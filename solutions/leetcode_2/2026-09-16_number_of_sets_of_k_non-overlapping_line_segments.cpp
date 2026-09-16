#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int numberOfSets(int n, int k) {
        const int MOD = 1000000007;
        vector<vector<int>> dp(n, vector<int>(k + 1, 0));
        vector<vector<int>> pref(n, vector<int>(k + 1, 0));
        for (int i = 0; i < n; ++i) {
            dp[i][0] = 1;
            pref[i][0] = (i ? (pref[i-1][0] + dp[i][0]) % MOD : dp[i][0]);
            for (int j = 1; j <= k; ++j) {
                long long add = (i > 0) ? pref[i-1][j-1] : 0;
                long long prev = (i > 0) ? dp[i-1][j] : 0;
                dp[i][j] = (add + prev) % MOD;
                pref[i][j] = ((i > 0 ? pref[i-1][j] : 0) + dp[i][j]) % MOD;
            }
        }
        return dp[n-1][k];
    }
};