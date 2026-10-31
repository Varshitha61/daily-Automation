#include <bits/stdc++.ndefine MOD 1000000007
using namespace std;

class Solution {
public:
    int m, n;
    vector<vector<int>> grid;
    vector<array<pair<int,int>,2>> memo; // flattened index
    vector<array<char,2>> vis;
    
    pair<int,int> solve(int i, int j, int d){
        int idx = i*n + j;
        if(vis[idx][d]) return memo[idx][d];
        vis[idx][d]=1;
        int ni = i + (d==1);
        int nj = j + (d==0);
        if(ni<0||ni>=m||nj<0||nj>=n){
            memo[idx][d] = {-1,-1};
            return memo[idx][d];
        }
        if(grid[ni][nj]==0){
            memo[idx][d] = {ni,nj};
            return memo[idx][d];
        }else{
            memo[idx][d] = solve(ni,nj,1-d);
            return memo[idx][d];
        }
    }
    
    int countPaths(vector<vector<int>>& g) {
        grid=g;
        m=grid.size(); n=grid[0].size();
        memo.assign(m*n, {{{-1,-1},{-1,-1}}});
        vis.assign(m*n, {{0,0}});
        vector<vector<int>> dp(m, vector<int>(n,0));
        dp[0][0]=1;
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                if(dp[i][j]==0) continue;
                for(int d=0;d<2;d++){
                    auto p=solve(i,j,d);
                    if(p.first==-1) continue;
                    int x=p.first, y=p.second;
                    dp[x][y] = (dp[x][y] + dp[i][j])%MOD;
                }
            }
        }
        return dp[m-1][n-1];
    }
};