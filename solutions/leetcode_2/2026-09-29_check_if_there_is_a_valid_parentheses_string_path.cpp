#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool hasValidPath(vector<vector<char>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        int maxLen = m + n; // enough for balance
        const int MAXB = 205; // > maxLen
        vector<vector<bitset<MAXB>>> dp(m, vector<bitset<MAXB>>(n));
        // start
        if (grid[0][0] == '(') dp[0][0].set(1);
        // else remains empty (invalid)
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == 0 && j == 0) continue;
                bitset<MAXB> prev;
                if (i > 0) prev |= dp[i-1][j];
                if (j > 0) prev |= dp[i][j-1];
                if (grid[i][j] == '(') {
                    dp[i][j] = (prev << 1);
                } else { // ')'
                    dp[i][j] = (prev >> 1);
                }
                // ensure balance never negative: shifting right already drops bit 0
                // also we can clear bits beyond maxLen, but bitset size is enough
            }
        }
        return dp[m-1][n-1].test(0);
    }
};