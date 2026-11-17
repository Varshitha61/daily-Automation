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
            for (int r = l; r < n; ++r) {
                int len = r - l + 1;
                if (len == 1) continue; // impossible
                int best = INF;
                for (int d : divisors[len]) {
                    int changes = 0;
                    for (int g = 0; g < d; ++g) {
                        // first index in substring with this modulo
                        int first = l + g;
                        if (first > r) continue;
                        int cnt = (r - first) / d + 1; // number of elements in this group
                        for (int t = 0; t < cnt / 2; ++t) {
                            int leftIdx = first + t * d;
                            int rightIdx = first + (cnt - 1 - t) * d;
                            if (s[leftIdx] != s[rightIdx]) ++changes;
                        }
                    }
                    best = min(best, changes);
                }
                cost[l][r] = best;
            }
        }
        // DP
        vector<vector<int>> dp(n + 1, vector<int>(k + 1, INF));
        dp[0][0] = 0;
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= k && j <= i; ++j) {
                for (int p = j - 1; p < i; ++p) {
                    if (dp[p][j - 1] == INF) continue;
                    int c = cost[p][i - 1];
                    if (c == INF) continue;
                    dp[i][j] = min(dp[i][j], dp[p][j - 1] + c);
                }
            }
        }
        return dp[n][k];
    }
};