#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minChanges(string s, int k) {
        int n = s.size();
        const int INF = 1e9;
        // precompute divisors for each length
        vector<vector<int>> divisors(n + 1);
        for (int len = 2; len <= n; ++len) {
            for (int d = 1; d < len; ++d) {
                if (len % d == 0) divisors[len].push_back(d);
            }
        }
        // cost[l][r]
        vector<vector<int>> cost(n, vector<int>(n, INF));
        for (int l = 0; l < n; ++l) {
            for (int r = l + 1; r < n; ++r) { // length >=2
                int len = r - l + 1;
                int best = INF;
                for (int d : divisors[len]) {
                    int m = len / d;
                    int cur = 0;
                    for (int offset = 0; offset < d; ++offset) {
                        for (int t = 0; t < m / 2; ++t) {
                            int pos1 = l + offset + t * d;
                            int pos2 = l + offset + (m - 1 - t) * d;
                            if (s[pos1] != s[pos2]) ++cur;
                        }
                    }
                    best = min(best, cur);
                }
                cost[l][r] = best;
            }
        }
        // dp[i][p]: first i chars (0..i-1) into p substrings
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, INF));
        dp[0][0] = 0;
        for (int i = 2; i <= n; ++i) { // i is length of prefix
            for (int p = 1; p <= k; ++p) {
                for (int j = i - 2; j >= 0; --j) { // substring [j, i-1] length >=2
                    if (dp[j][p - 1] == INF) continue;
                    int c = cost[j][i - 1];
                    if (c == INF) continue;
                    dp[i][p] = min(dp[i][p], dp[j][p - 1] + c);
                }
            }
        }
        return dp[n][k];
    }
};