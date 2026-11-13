#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int MAXA = 1000000;
    vector<bool> isPrime(MAXA + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int p = 2; p * p <= MAXA; ++p) if (isPrime[p]) {
        for (int q = p * p; q <= MAXA; q += p) isPrime[q] = false;
    }
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, e;
        cin >> n >> e;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        vector<int> left(n, 0), right(n, 0);
        for (int i = 0; i < n; ++i) {
            if (a[i] == 1) {
                left[i] = (i - e >= 0 ? left[i - e] : 0) + 1;
            }
        }
        for (int i = n - 1; i >= 0; --i) {
            if (a[i] == 1) {
                right[i] = (i + e < n ? right[i + e] : 0) + 1;
            }
        }
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            if (a[i] <= MAXA && isPrime[a[i]]) {
                long long L = (i - e >= 0) ? left[i - e] : 0;
                long long R = (i + e < n) ? right[i + e] : 0;
                ans += (L + 1) * (R + 1) - 1;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}