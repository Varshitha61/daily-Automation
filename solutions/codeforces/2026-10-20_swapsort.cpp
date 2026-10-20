#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    vector<long long> b = a;
    sort(b.begin(), b.end());
    vector<pair<int,int>> ops;
    for (int i = 0; i < n; ++i) {
        if (a[i] == b[i]) continue;
        int j = i + 1;
        while (j < n && a[j] != b[i]) ++j;
        if (j == n) continue; // should not happen
        swap(a[i], a[j]);
        ops.emplace_back(i, j);
    }
    cout << ops.size() << "\n";
    for (auto &p : ops) {
        cout << p.first << " " << p.second << "\n";
    }
    return 0;
}