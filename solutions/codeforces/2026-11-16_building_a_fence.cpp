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
        vector<long long> h(n);
        for (int i = 0; i < n; ++i) cin >> h[i];
        long long low = h[0], high = h[0];
        bool ok = true;
        for (int i = 1; i < n; ++i) {
            long long newLow = max(h[i], low - (k - 1));
            long long newHigh = min(h[i] + (k - 1), high + (k - 1));
            if (newLow > newHigh) {
                ok = false;
                break;
            }
            low = newLow;
            high = newHigh;
        }
        if (ok && low <= h.back() && h.back() <= high) cout << "YES\n";
        else cout << "NO\n";
    }
    return 0;
}