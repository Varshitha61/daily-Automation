#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<long long> ans(n);
    vector<int> nonInt(n, 0);
    long long sumFloor = 0;
    const long double EPS = 1e-12L;
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        long double x = stold(s);
        long double fl_ld = floor(x);
        long long fl = (long long)fl_ld;
        if (fabsl(x - fl_ld) < EPS) {
            ans[i] = fl;
        } else {
            ans[i] = fl;
            nonInt[i] = 1;
        }
        sumFloor += ans[i];
    }
    long long need = -sumFloor; // number of +1 adjustments needed
    for (int i = 0; i < n && need > 0; ++i) {
        if (nonInt[i]) {
            ans[i] += 1;
            --need;
        }
    }
    for (int i = 0; i < n; ++i) {
        cout << ans[i] << "\n";
    }
    return 0;
}