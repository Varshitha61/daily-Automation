#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<long long> a(m);
        for (int i = 0; i < m; ++i) cin >> a[i];
        vector<pair<long long,long long>> v;
        v.reserve(m);
        for (auto x: a) v.emplace_back(x/100, x);
        sort(v.begin(), v.end(), [](auto &p1, auto &p2){
            if (p1.first != p2.first) return p1.first < p2.first;
            return p1.second < p2.second;
        });
        vector<long long> low, high;
        low.reserve(n);
        high.reserve(n);
        for (int i = 0; i < n; ++i) low.push_back(v[i].second);
        for (int i = 0; i < n; ++i) high.push_back(v[m - n + i].second);
        for (int i = 0; i < n; ++i) {
            cout << low[i] << ' ' << high[i] << ' '
                 << low[i] << ' ' << high[i] << ' '
                 << low[i] << ' ' << high[i] << "\n";
        }
    }
    return 0;
}