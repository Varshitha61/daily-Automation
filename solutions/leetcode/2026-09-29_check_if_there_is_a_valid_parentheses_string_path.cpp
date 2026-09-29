#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool hasValidPath(vector<string>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        if (grid[0][0] == ')') return false;
        int maxBal = m + n; // maximum possible balance
        vector<vector<vector<char>>> dp(m, vector<vector<char>>(n, vector<char>(maxBal + 1, 0)));
        dp[0][0][1] = 1; // starting with '(' gives balance 1

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                for (int b = 0; b <= maxBal; ++b) {
                    if (!dp[i][j][b]) continue;
                    // move down
                    if (i + 1 < m) {
                        int nb = b + (grid[i+1][j] == '(' ? 1 : -1);
                        if (nb >= 0 && nb <= maxBal) dp[i+1][j][nb] = 1;
                    }
                    // move right
                    if (j + 1 < n) {
                        int nb = b + (grid[i][j+1] == '(' ? 1 : -1);
                        if (nb >= 0 && nb <= maxBal) dp[i][j+1][nb] = 1;
                    }
                }
            }
        }
        return dp[m-1][n-1][0];
    }
};