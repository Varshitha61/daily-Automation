#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int countIslandsDivisibleByK(vector<vector<int>>& grid, int k) {
        int m = grid.size();
        if (m == 0) return 0;
        int n = grid[0].size();
        vector<vector<char>> vis(m, vector<char>(n, 0));
        int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        int ans = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (grid[i][j] > 0 && !vis[i][j]) {
                    long long sum = 0;
                    queue<pair<int,int>> q;
                    q.emplace(i,j);
                    vis[i][j] = 1;
                    while (!q.empty()) {
                        auto [x,y] = q.front(); q.pop();
                        sum += grid[x][y];
                        for (auto &d: dirs) {
                            int nx = x + d[0], ny = y + d[1];
                            if (nx>=0 && nx<m && ny>=0 && ny<n && grid[nx][ny]>0 && !vis[nx][ny]) {
                                vis[nx][ny] = 1;
                                q.emplace(nx,ny);
                            }
                        }
                    }
                    if (sum % k == 0) ++ans;
                }
            }
        }
        return ans;
    }
};