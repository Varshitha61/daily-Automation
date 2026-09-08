#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n;
    if(!(cin >> n)) return 0;
    long long total = 0;
    long long pow10 = 1;
    for(int d = 1; d <= 12; ++d) {
        long long start = pow10;
        long long end = pow10 * 10 - 1;
        if (d == 1) start = 1;
        if (start > n) break;
        long long l = max(start, 1LL);
        long long r = min(end, n);
        if (r >= l && d >= 4) {
            long long cnt = r - l + 1;
            long long commas = (d - 1) / 3;
            total += cnt * commas;
        }
        pow10 *= 10;
    }
    cout << total;
    return 0;
}