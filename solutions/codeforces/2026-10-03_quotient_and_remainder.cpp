#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n;
        long long k;
        cin >> n >> k;
        vector<long long> q(n), r(n);
        for (int i = 0; i < n; ++i) cin >> q[i];
        for (int i = 0; i < n; ++i) cin >> r[i];
        vector<long long> limits;
        limits.reserve(n);
        for (long long qi : q) {
            if (k <= qi) continue; // (k - qi) negative or zero, limit <0
            long long limit = (k - qi) / (qi + 1);
            if (limit >= 1) limits.push_back(limit);
        }
        sort(limits.begin(), limits.end());
        sort(r.begin(), r.end());
        size_t ptr = 0;
        long long ans = 0;
        for (long long lim : limits) {
            if (ptr < r.size() && r[ptr] <= lim) {
                ++ans;
                ++ptr;
            }
        }
        cout << ans << "\n";
    }
    return 0;
}