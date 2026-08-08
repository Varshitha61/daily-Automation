#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <climits>

using namespace std;

const int MAX_N = 155;
const int MAX_M = 155;
const int MAX_DIR = 2;
const int INF = INT_MAX;

int n, m;
char grid[MAX_N][MAX_M];
int dist[MAX_N][MAX_M][MAX_DIR];

int dx[MAX_DIR] = {0, 0};
int dy[MAX_DIR] = {1, -1};

int main() {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }
    }

    memset(dist, 0x3f, sizeof(dist));
    dist[0][0][0] = 0;

    queue<pair<pair<int, int>, int>> q;
    q.push({{0, 0}, 0});

    while (!q.empty()) {
        int x = q.front().first.first;
        int y = q.front().first.second;
        int dir = q.front().second;
        q.pop();

        if (y + dy[dir] >= 0 && y + dy[dir] < m) {
            if (dist[x][y + dy[dir]][dir] > dist[x][y][dir]) {
                dist[x][y + dy[dir]][dir] = dist[x][y][dir];
                q.push({{x, y + dy[dir]}, dir});
            }
        }

        if (x + 1 < n) {
            if (dist[x + 1][y][1 - dir] > dist[x][y][dir] + 1) {
                dist[x + 1][y][1 - dir] = dist[x][y][dir] + 1;
                q.push({{x + 1, y}, 1 - dir});
            }
        }
    }

    int ans = INF;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 'G') {
                continue;
            }
            ans = min(ans, min(dist[i][j][0], dist[i][j][1]));
        }
    }

    cout << ans << endl;

    return 0;
}