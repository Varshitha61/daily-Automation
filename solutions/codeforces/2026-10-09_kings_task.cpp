#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin >> n)) return 0;
    int m = 2 * n;
    vector<int> p(m);
    for (int i = 0; i < m; ++i) cin >> p[i];
    auto is_sorted = [&](const vector<int>& v) {
        for (int i = 0; i < (int)v.size(); ++i)
            if (v[i] != i + 1) return false;
        return true;
    };
    if (is_sorted(p)) {
        cout << 0 << "\n";
        return 0;
    }
    auto applyA = [&](vector<int>& v) {
        for (int i = 0; i + 1 < m; i += 2)
            swap(v[i], v[i + 1]);
    };
    auto applyB = [&](vector<int>& v) {
        for (int i = 0; i < n; ++i)
            swap(v[i], v[i + n]);
    };
    const int LIM = 2 * n + 5;
    int ans = INT_MAX;
    // pattern 1: A, B, A, B, ...
    vector<int> cur = p;
    for (int step = 1; step <= LIM; ++step) {
        if (step % 2 == 1) applyA(cur);
        else applyB(cur);
        if (is_sorted(cur)) {
            ans = min(ans, step);
            break;
        }
    }
    // pattern 2: B, A, B, A, ...
    cur = p;
    for (int step = 1; step <= LIM; ++step) {
        if (step % 2 == 1) applyB(cur);
        else applyA(cur);
        if (is_sorted(cur)) {
            ans = min(ans, step);
            break;
        }
    }
    if (ans == INT_MAX) cout << -1 << "\n";
    else cout << ans << "\n";
    return 0;
}