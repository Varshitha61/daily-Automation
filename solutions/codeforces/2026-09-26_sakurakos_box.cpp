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
    const long long inv2 = (MOD + 1) / 2; // 500000004
    int T;
    if(!(cin >> T)) return 0;
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
        numerator = numerator * inv2 % MOD; // divide by 2
        long long denom = ( (long long)n * (n - 1) ) % MOD;
        denom = denom * inv2 % MOD; // C(n,2)
        long long ans = numerator * modpow(denom, MOD - 2) % MOD;
        cout << ans << '\n';
    }
    return 0;
}