#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n, m, a, b;
    if (!(cin >> n >> m >> a >> b)) return 0;
    long long cost_all_single = n * a;
    long long cost_mix = (n / m) * b + (n % m) * a;
    long long cost_extra = ((n + m - 1) / m) * b;
    long long ans = min({cost_all_single, cost_mix, cost_extra});
    cout << ans << "\n";
    return 0;
}