#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n;
    if(!(cin >> n)) return 0;
    long long total = 0;
    for (long long i = 1; i <= n; ++i) {
        int digits = 0;
        long long x = i;
        while (x) {
            ++digits;
            x /= 10;
        }
        if (digits >= 4) total += (digits - 1) / 3;
    }
    cout << total;
    return 0;
}