#include <iostream>
#include <vector>
#include <string>

using namespace std;

void solve() {
    int n, m, k;
    cin >> n >> m >> k;
    vector<vector<char>> a(n, vector<char>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> a[i][j];
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (a[i][j] == '*') {
                bool ok = false;
                for (int d = k; d <= min(i, j, n - i - 1, m - j - 1); d++) {
                    bool good = true;
                    for (int h = 0; h <= d; h++) {
                        if (a[i - h][j + h] != '*' || a[i - h][j - h] != '*') {
                            good = false;
                            break;
                        }
                    }
                    if (good) {
                        ok = true;
                        for (int h = 0; h <= d; h++) {
                            a[i - h][j + h] = '.';
                            a[i - h][j - h] = '.';
                        }
                        a[i][j] = '.';
                        break;
                    }
                }
                if (!ok) {
                    cout << "NO" << endl;
                    return;
                }
            }
        }
    }
    cout << "YES" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}