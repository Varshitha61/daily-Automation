#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

const long long MOD = 1e9 + 7;

long long powmod(long long a, long long b) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

long long inv(long long a) {
    return powmod(a, MOD - 2);
}

long long C(long long n, long long k) {
    long long res = 1;
    for (int i = 1; i <= k; i++) {
        res = res * (n - i + 1) % MOD;
        res = res * inv(i) % MOD;
    }
    return res;
}

void solve() {
    long long n, k;
    cin >> n >> k;
    vector<long long> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    long long ans = 0;
    for (int i = 0; i < n; i++) {
        long long cnt = 0;
        for (int j = 0; j < n; j++) {
            if (a[j] == 1) cnt++;
        }
        long long x = (k + 1) / 2;
        long long y = k - x;
        long long res = C(cnt, x) * C(n - cnt, y) % MOD;
        ans = (ans + res * a[i]) % MOD;
    }
    cout << ans << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}