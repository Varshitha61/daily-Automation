#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long p, d;
    if (!(cin >> p >> d)) return 0;
    vector<long long> pow10(19);
    pow10[0] = 1;
    for (int i = 1; i < 19; ++i) pow10[i] = pow10[i-1] * 10LL;
    long long bestX = p;
    int bestK = 0;
    for (int k = 0; k < 19; ++k) {
        long long mod = pow10[k];
        long long r = mod - 1; // desired remainder
        long long t = p % mod;
        long long diff = (t - r + mod) % mod;
        long long x = p - diff;
        if (x < p - d) continue;
        if (k > bestK || (k == bestK && x > bestX)) {
            bestK = k;
            bestX = x;
        }
    }
    cout << bestX << "\n";
    return 0;
}