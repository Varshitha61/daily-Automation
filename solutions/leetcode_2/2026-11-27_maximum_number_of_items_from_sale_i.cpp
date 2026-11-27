#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maximumItems(vector<vector<int>>& items, int budget) {
        int n = items.size();
        vector<int> factor(n), price(n);
        for (int i = 0; i < n; ++i) {
            factor[i] = items[i][0];
            price[i] = items[i][1];
        }
        vector<int> freeCnt(n, 0);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) if (i != j) {
                if (factor[j] % factor[i] == 0) freeCnt[i]++;
            }
        }
        const int INF_NEG = -1e9;
        vector<int> dp(budget + 1, INF_NEG);
        dp[0] = 0;
        // first copy (0-1 knapsack)
        for (int i = 0; i < n; ++i) {
            int c = price[i];
            int val = 1 + freeCnt[i];
            for (int b = budget; b >= c; --b) {
                if (dp[b - c] != INF_NEG)
                    dp[b] = max(dp[b], dp[b - c] + val);
            }
        }
        // additional copies (unbounded knapsack)
        for (int i = 0; i < n; ++i) {
            int c = price[i];
            for (int b = c; b <= budget; ++b) {
                if (dp[b - c] != INF_NEG)
                    dp[b] = max(dp[b], dp[b - c] + 1);
            }
        }
        return *max_element(dp.begin(), dp.end());
    }
};