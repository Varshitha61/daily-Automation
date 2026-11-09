#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n, m, k;
        cin >> n >> m >> k;
        vector<string> g(n);
        for (int i = 0; i < n; ++i) cin >> g[i];
        vector<vector<int>> ul(n, vector<int>(m, 0));
        vector<vector<int>> ur(n, vector<int>(m, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                if (g[i][j] == '*') {
                    ul[i][j] = 1;
                    if (i > 0 && j > 0) ul[i][j] += ul[i-1][j-1];
                }
            }
        }
        for (int i = 0; i < n; ++i) {
            for (int j = m-1; j >= 0; --j) {
                if (g[i][j] == '*') {
                    ur[i][j] = 1;
                    if (i > 0 && j+1 < m) ur[i][j] += ur[i-1][j+1];
                }
            }
        }
        vector<vector<bool>> covered(n, vector<bool>(m, false));
        for (int i = n-1; i >= 0; --i) {
            for (int j = 0; j < m; ++j) {
                if (g[i][j] != '*') continue;
                int maxSize = min(ul[i][j], ur[i][j]) - 1;
                if (maxSize >= k) {
                    for (int h = 0; h <= maxSize; ++h) {
                        covered[i-h][j-h] = true;
                        covered[i-h][j+h] = true;
                    }
                }
            }
        }
        bool ok = true;
        for (int i = 0; i < n && ok; ++i)
            for (int j = 0; j < m; ++j)
                if (g[i][j] == '*' && !covered[i][j]) {
                    ok = false;
                    break;
                }
        cout << (ok ? "YES\n" : "NO\n");
    }
    return 0;
}