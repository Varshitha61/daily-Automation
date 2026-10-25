#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int closedIsland(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        auto bfs = [&](int si, int sj) {
            queue<pair<int,int>> q;
            q.emplace(si, sj);
            grid[si][sj] = 1;
            while (!q.empty()) {
                auto [i, j] = q.front(); q.pop();
                const int di[4] = {-1,1,0,0};
                const int dj[4] = {0,0,-1,1};
                for (int d=0; d<4; ++d) {
                    int ni = i + di[d], nj = j + dj[d];
                    if (ni>=0 && ni<n && nj>=0 && nj<m && grid[ni][nj]==0) {
                        grid[ni][nj]=1;
                        q.emplace(ni,nj);
                    }
                }
            }
        };
        // Remove border-connected lands
        for (int i=0;i<n;i++) {
            if (grid[i][0]==0) bfs(i,0);
            if (grid[i][m-1]==0) bfs(i,m-1);
        }
        for (int j=0;j<m;j++) {
            if (grid[0][j]==0) bfs(0,j);
            if (grid[n-1][j]==0) bfs(n-1,j);
        }
        int closed = 0;
        for (int i=1;i<n-1;i++) {
            for (int j=1;j<m-1;j++) {
                if (grid[i][j]==0) {
                    ++closed;
                    bfs(i,j);
                }
            }
        }
        return closed;
    }
};