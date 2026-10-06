#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<long long> x(n + 1);
    for (int i = 1; i <= n; ++i) cin >> x[i];
    vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; ++i) pref[i] = pref[i - 1] + x[i];
    int m = n - k + 1; // number of possible segments
    vector<long long> seg(m + 1);
    for (int i = 1; i <= m; ++i) {
        seg[i] = pref[i + k - 1] - pref[i - 1];
    }
    vector<int> best_left(m + 1);
    best_left[1] = 1;
    for (int i = 2; i <= m; ++i) {
        if (seg[i] > seg[best_left[i - 1]]) best_left[i] = i;
        else if (seg[i] == seg[best_left[i - 1]]) best_left[i] = best_left[i - 1]; // keep smaller index
        else best_left[i] = best_left[i - 1];
    }
    long long best_total = -1;
    int ans_a = 1, ans_b = k + 1;
    for (int b = k + 1; b <= m; ++b) {
        int a = best_left[b - k];
        long long total = seg[a] + seg[b];
        if (total > best_total ||
            (total == best_total && (a < ans_a || (a == ans_a && b < ans_b)))) {
            best_total = total;
            ans_a = a;
            ans_b = b;
        }
    }
    cout << ans_a << ' ' << ans_b << "\n";
    return 0;
}