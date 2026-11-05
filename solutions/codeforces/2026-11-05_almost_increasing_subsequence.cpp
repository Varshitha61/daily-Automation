#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];
    vector<int> b(n, 0), start(n, 0);
    for (int i = 1; i < n; ++i) {
        b[i] = (a[i] >= a[i + 1]) ? 1 : 0;
        if (b[i] && (i == 1 || b[i - 1] == 0)) start[i] = 1;
    }
    vector<long long> prefB(n, 0), prefStart(n, 0);
    for (int i = 1; i < n; ++i) {
        prefB[i] = prefB[i - 1] + b[i];
        prefStart[i] = prefStart[i - 1] + start[i];
    }
    while (q--) {
        int l, r;
        cin >> l >> r;
        long long len = r - l + 1;
        if (l == r) {
            cout << 1 << '\n';
            continue;
        }
        int L = l;
        int R = r - 1;
        long long cntOnes = prefB[R] - prefB[L - 1];
        long long segCnt = prefStart[R] - prefStart[L - 1];
        long long ans = len - cntOnes + segCnt;
        cout << ans << '\n';
    }
    return 0;
}