#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int numberOfSets(int n, int k) {
        const int MOD = 1000000007;
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0));
        vector<vector<int>> pref(n + 1, vector<int>(k + 1, 0));
        dp[0][0] = 1;
        pref[0][0] = 1;
        for (int i = 1; i <= n; ++i) {
            dp[i][0] = 1;
            pref[i][0] = (pref[i - 1][0] + dp[i][0]) % MOD;
            for (int j = 1; j <= k; ++j) {
                long long term = pref[i - 1][j - 1];
                if (j == 1) term = (term - 1 + MOD) % MOD; // subtract dp[0][0]
                // else subtract 0
                dp[i][j] = ( (long long)dp[i - 1][j] + term ) % MOD;
                pref[i][j] = (pref[i - 1][j] + dp[i][j]) % MOD;
            }
        }
        return dp[n][k];
    }
};