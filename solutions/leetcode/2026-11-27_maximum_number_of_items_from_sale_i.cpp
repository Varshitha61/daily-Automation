#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxNumberOfItems(vector<vector<int>>& items, int budget) {
        int n = items.size();
        vector<int> factor(n), price(n);
        int min_price = INT_MAX;
        for (int i = 0; i < n; ++i) {
            factor[i] = items[i][0];
            price[i] = items[i][1];
            min_price = min(min_price, price[i]);
        }
        vector<int> cntDiv(n, 0);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) if (i != j) {
                if (factor[j] % factor[i] == 0) cntDiv[i]++;
            }
        }
        vector<int> base(n);
        for (int i = 0; i < n; ++i) base[i] = 1 + cntDiv[i];
        const int INF_NEG = -1e9;
        vector<int> dp(budget + 1, INF_NEG);
        dp[0] = 0;
        for (int i = 0; i < n; ++i) {
            int w = price[i], v = base[i];
            for (int c = budget - w; c >= 0; --c) {
                if (dp[c] != INF_NEG) {
                    dp[c + w] = max(dp[c + w], dp[c] + v);
                }
            }
        }
        int ans = budget / min_price; // buy only cheapest copies
        for (int c = 0; c <= budget; ++c) {
            if (dp[c] != INF_NEG) {
                int extra = (budget - c) / min_price;
                ans = max(ans, dp[c] + extra);
            }
        }
        return ans;
    }
};