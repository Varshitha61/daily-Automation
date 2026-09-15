#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    if (!(cin >> s)) return 0;
    int k;
    cin >> k;
    int n = s.size();
    vector<vector<char>> pal(n, vector<char>(n, 0));
    for (int i = n - 1; i >= 0; --i) {
        for (int j = i; j < n; ++j) {
            if (s[i] == s[j] && (j - i < 2 || pal[i + 1][j - 1]))
                pal[i][j] = 1;
        }
    }
    vector<int> dp(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        dp[i + 1] = max(dp[i + 1], dp[i]); // skip character i
        for (int j = i + k - 1; j < n; ++j) {
            if (pal[i][j]) {
                dp[j + 1] = max(dp[j + 1], dp[i] + 1);
            }
        }
    }
    cout << dp[n];
    return 0;
}