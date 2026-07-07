#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

int maxSafeness(vector<vector<int>>& grid) {
    int n = grid.size();
    vector<vector<int>> distance(n, vector<int>(n, INT_MAX));
    queue<pair<int, int>> q;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 1) {
                q.push({i, j});
                distance[i][j] = 0;
            }
        }
    }
    vector<int> dx = {-1, 1, 0, 0};
    vector<int> dy = {0, 0, -1, 1};
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < n && ny >= 0 && ny < n) {
                if (distance[nx][ny] > distance[x][y] + 1) {
                    distance[nx][ny] = distance[x][y] + 1;
                    q.push({nx, ny});
                }
            }
        }
    }
    int ans = INT_MIN;
    vector<vector<int>> dp(n, vector<int>(n, INT_MIN));
    dp[0][0] = distance[0][0];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i > 0) {
                dp[i][j] = max(dp[i][j], dp[i - 1][j]);
            }
            if (j > 0) {
                dp[i][j] = max(dp[i][j], dp[i][j - 1]);
            }
            if (i < n - 1) {
                dp[i + 1][j] = max(dp[i + 1][j], dp[i][j]);
            }
            if (j < n - 1) {
                dp[i][j + 1] = max(dp[i][j + 1], dp[i][j]);
            }
        }
    }
    return dp[n - 1][n - 1];
}

int main() {
    vector<vector<int>> grid = {{1, 0, 0}, {0, 0, 0}, {0, 0, 1}};
    cout << maxSafeness(grid) << endl;
    return 0;
}