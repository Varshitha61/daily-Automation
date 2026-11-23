#include <bits/stdc++.h>
using namespace std;

long long ceil_div2(long long x) {
    if (x >= 0) return (x + 1) / 2;
    else return x / 2; // trunc toward zero gives ceil for negative
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n, a, b, c;
    if (!(cin >> n >> a >> b >> c)) return 0;
    long long target = 2 * n;
    long long maxZ = min(c, n / 2); // because 4z <= 2n -> z <= n/2
    long long ans = 0;
    for (long long z = 0; z <= maxZ; ++z) {
        long long remaining = target - 4 * z;
        if (remaining < 0) break;
        long long highY = min(b, remaining / 2);
        long long lowY = max(0LL, ceil_div2(remaining - a));
        if (lowY <= highY) ans += (highY - lowY + 1);
    }
    cout << ans << "\n";
    return 0;
}