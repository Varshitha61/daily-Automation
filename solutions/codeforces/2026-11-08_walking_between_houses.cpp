#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n, k, s;
    if (!(cin >> n >> k >> s)) return 0;
    long long minDist = k * 1LL;
    long long maxDist = k * (n - 1);
    if (s < minDist || s > maxDist) {
        cout << "NO\n";
        return 0;
    }
    vector<long long> ans;
    ans.reserve(k);
    long long cur = 1;
    long long rem = s;
    for (long long i = 1; i <= k; ++i) {
        long long remaining = k - i + 1;
        long long low = max(1LL, rem - (remaining - 1) * (n - 1));
        long long high = min(n - 1, rem - (remaining - 1) * 1LL);
        if (low > high) {
            cout << "NO\n";
            return 0;
        }
        long long d = high; // take as large as possible
        long long nxt;
        if (cur + d <= n) nxt = cur + d;
        else nxt = cur - d;
        ans.push_back(nxt);
        cur = nxt;
        rem -= d;
    }
    if (rem != 0) {
        cout << "NO\n";
        return 0;
    }
    cout << "YES\n";
    for (size_t i = 0; i < ans.size(); ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << "\n";
    return 0;
}