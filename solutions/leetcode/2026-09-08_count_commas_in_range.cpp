#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n;
    if(!(cin >> n)) return 0;
    long long total = 0;
    long long start = 1;
    for (int d = 1; start <= n; ++d) {
        long long end = min(n, start * 10 - 1);
        long long cnt = end - start + 1;
        long long commas_per_number = (d - 1) / 3;
        total += cnt * commas_per_number;
        start *= 10;
    }
    cout << total;
    return 0;
}