#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minXorPath(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        const int MAXV = 1024; // 2^10
        vector<vector<array<char, MAXV>>> dp(m, vector<array<char, MAXV>>(n));
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                dp[i][j].fill(0);
        dp[0][0][grid[0][0]] = 1;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == 0 && j == 0) continue;
                for (int v = 0; v < MAXV; ++v) {
                    bool reachable = false;
                    if (i > 0 && dp[i-1][j][v]) reachable = true;
                    if (j > 0 && dp[i][j-1][v]) reachable = true;
                    if (reachable) {
                        dp[i][j][v ^ grid[i][j]] = 1;
                    }
                }
            }
        }
        for (int v = 0; v < MAXV; ++v) {
            if (dp[m-1][n-1][v]) return v;
        }
        return 0; // should never reach here
    }
};