#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        if (n <= 2) {
            cout << 0 << "\n";
            continue;
        }
        int best = 1; // at least one point can stay
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                long long dy = a[j] - a[i];
                long long dx = j - i;
                int cnt = 0;
                for (int k = 0; k < n; ++k) {
                    if ((a[k] - a[i]) * dx == (long long)(k - i) * dy) ++cnt;
                }
                best = max(best, cnt);
            }
        }
        cout << n - best << "\n";
    }
    return 0;
}