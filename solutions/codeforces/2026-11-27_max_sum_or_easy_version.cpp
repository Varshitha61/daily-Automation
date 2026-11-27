#include <bits/stdc++.h>
using namespace std;

void build(int r, vector<int>& ans, long long& total) {
    if (r < 0) return;
    if (r == 0) {
        ans[0] = 0;
        return;
    }
    int msb = 31 - __builtin_clz(r);
    int mask = (1 << (msb + 1)) - 1;
    int L = mask - r; // start of segment where complements exist
    int segSize = r - L + 1;
    total += 1LL * segSize * mask;
    for (int i = L; i <= r; ++i) {
        ans[i] = mask ^ i;
    }
    build(L - 1, ans, total);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int l, r;
        cin >> l >> r; // l is always 0 in this version
        int n = r - l + 1;
        vector<int> ans(r + 1);
        long long total = 0;
        build(r, ans, total);
        cout << total << "\n";
        for (int i = 0; i <= r; ++i) {
            cout << ans[i] << (i == r ? '\n' : ' ');
        }
    }
    return 0;
}