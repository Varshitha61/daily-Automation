#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<string> g(n);
        for (int i = 0; i < n; ++i) cin >> g[i];
        vector<vector<int>> dp(n, vector<int>(m, 0));
        long long ans = 0;
        for (int i = n - 1; i >= 0; --i) {
            for (int j = 0; j < m; ++j) {
                if (g[i][j] == '*') {
                    if (i == n - 1 || j == 0 || j == m - 1) {
                        dp[i][j] = 1;
                    } else {
                        dp[i][j] = 1 + min({dp[i + 1][j - 1], dp[i + 1][j], dp[i + 1][j + 1]});
                    }
                    ans += dp[i][j];
                }
            }
        }
        cout << ans << "\n";
    }
    return 0;
}