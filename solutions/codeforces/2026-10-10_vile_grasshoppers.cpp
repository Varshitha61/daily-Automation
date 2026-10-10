#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long p, y;
    if (!(cin >> p >> y)) return 0;
    for (long long i = y; i > p; --i) {
        bool ok = true;
        long long limit = min(p, (long long)sqrt((long double)i));
        for (long long d = 2; d <= limit; ++d) {
            if (i % d == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            cout << i << "\n";
            return 0;
        }
    }
    cout << -1 << "\n";
    return 0;
}