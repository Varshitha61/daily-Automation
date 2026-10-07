#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long l, r, m;
        cin >> l >> r >> m;
        long long diffRange = r - l;
        for (long long a = l; a <= r; ++a) {
            long long n = m / a;
            for (long long cand = max(1LL, n); cand <= n + 1; ++cand) {
                if (cand <= 0) continue;
                long long diff = m - cand * a; // need b - c = diff
                if (diff >= -diffRange && diff <= diffRange) {
                    long long b, c;
                    if (diff >= 0) {
                        b = l + diff;
                        c = l;
                    } else {
                        b = l;
                        c = l - diff; // diff negative, so add
                    }
                    if (b >= l && b <= r && c >= l && c <= r) {
                        cout << a << ' ' << b << ' ' << c << "\n";
                        goto nextcase;
                    }
                }
            }
        }
        nextcase: ;
    }
    return 0;
}