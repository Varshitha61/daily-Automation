#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s, t;
    if(!(cin >> s)) return 0;
    if(!(cin >> t)) return 0;
    int n = s.size(), m = t.size();
    if (m > n) {
        cout << 0;
        return 0;
    }
    vector<int> dp(m + 1, 0);
    dp[0] = 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = m; j >= 1; --j) {
            if (s[i - 1] == t[j - 1]) {
                dp[j] = dp[j] + dp[j - 1];
            }
        }
    }
    cout << dp[m];
    return 0;
}