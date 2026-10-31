#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    static const int MOD = 1000000007;
    int m, n;
    vector<vector<int>> grid;
    vector<array<pair<int,int>,2>> memo; // flattened index

    pair<int,int> go(int i, int j, int d){
        if(i<0||i>=m||j<0||j>=n) return {-1,-1};
        int idx = i*n + j;
        if(memo[idx][d].first != -2) return memo[idx][d];
        int ci=i, cj=j, cd=d;
        while (true) {
            if(ci<0||ci>=m||cj<0||cj>=n){
                memo[idx][d] = {-1,-1};
                break;
            }
            if(grid[ci][cj]==0){
                memo[idx][d] = {ci,cj};
                break;
            }
            // mirror
            int nd = 1 - cd;
            if(nd==0) ++cj; else ++ci;
            cd = nd;
        }
        return memo[idx][d];
    }

    int countPaths(vector<vector<int>>& g) {
        grid = g;
        m = grid.size();
        n = grid[0].size();
        memo.assign(m*n, {{{-2,-2},{-2,-2}}});
        vector<vector<int>> dp(m, vector<int>(n,0));
        dp[0][0]=1;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if(dp[i][j]==0) continue;
                // move right
                if(j+1<n){
                    auto p = go(i, j+1, 0);
                    if(p.first!=-1){
                        dp[p.first][p.second] = (dp[p.first][p.second] + dp[i][j])%MOD;
                    }
                }
                // move down
                if(i+1<m){
                    auto p = go(i+1, j, 1);
                    if(p.first!=-1){
                        dp[p.first][p.second] = (dp[p.first][p.second] + dp[i][j])%MOD;
                    }
                }
            }
        }
        return dp[m-1][n-1];
    }
};