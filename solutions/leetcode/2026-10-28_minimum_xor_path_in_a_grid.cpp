#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minXorPath(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        const int MAXV = 1024;
        vector<vector<bitset<MAXV>>> dp(m, vector<bitset<MAXV>>(n));
        dp[0][0][grid[0][0]] = 1;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i + 1 < m) {
                    int val = grid[i+1][j];
                    for (int v = dp[i][j]._Find_first(); v < MAXV; v = dp[i][j]._Find_next(v)) {
                        dp[i+1][j][v ^ val] = 1;
                    }
                }
                if (j + 1 < n) {
                    int val = grid[i][j+1];
                    for (int v = dp[i][j]._Find_first(); v < MAXV; v = dp[i][j]._Find_next(v)) {
                        dp[i][j+1][v ^ val] = 1;
                    }
                }
            }
        }
        for (int v = 0; v < MAXV; ++v) {
            if (dp[m-1][n-1][v]) return v;
        }
        return 0; // should never reach
    }
};