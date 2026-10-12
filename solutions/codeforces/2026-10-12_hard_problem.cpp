#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin >> n)) return 0;
    vector<long long> c(n);
    for (int i = 0; i < n; ++i) cin >> c[i];
    vector<string> s(n), rev(n);
    for (int i = 0; i < n; ++i) {
        cin >> s[i];
        rev[i] = s[i];
        reverse(rev[i].begin(), rev[i].end());
    }
    const long long INF = (1LL<<60);
    vector<array<long long,2>> dp(n, {INF, INF});
    dp[0][0] = 0;
    dp[0][1] = c[0];
    for (int i = 1; i < n; ++i) {
        // previous not reversed -> s[i-1]
        // previous reversed -> rev[i-1]
        // current not reversed -> s[i]
        // current reversed -> rev[i]
        if (dp[i-1][0] < INF && s[i-1] <= s[i])
            dp[i][0] = min(dp[i][0], dp[i-1][0]);
        if (dp[i-1][0] < INF && s[i-1] <= rev[i])
            dp[i][1] = min(dp[i][1], dp[i-1][0] + c[i]);
        if (dp[i-1][1] < INF && rev[i-1] <= s[i])
            dp[i][0] = min(dp[i][0], dp[i-1][1]);
        if (dp[i-1][1] < INF && rev[i-1] <= rev[i])
            dp[i][1] = min(dp[i][1], dp[i-1][1] + c[i]);
    }
    long long ans = min(dp[n-1][0], dp[n-1][1]);
    if (ans >= INF) cout << -1 << "\n";
    else cout << ans << "\n";
    return 0;
}