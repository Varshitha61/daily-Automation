#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

long long modpow(long long a, long long e) {
    long long r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    const long long inv2 = (MOD + 1) / 2;
    while (T--) {
        int n;
        cin >> n;
        long long sum = 0, sumsq = 0;
        for (int i = 0; i < n; ++i) {
            long long x;
            cin >> x;
            x %= MOD;
            sum = (sum + x) % MOD;
            sumsq = (sumsq + x * x) % MOD;
        }
        long long numerator = ( (sum * sum) % MOD - sumsq + MOD ) % MOD;
        numerator = numerator * inv2 % MOD;
        long long denominator = ( (long long)n * (n - 1) ) % MOD;
        denominator = denominator * inv2 % MOD;
        long long ans = numerator * modpow(denominator, MOD - 2) % MOD;
        cout << ans << '\n';
    }
    return 0;
}