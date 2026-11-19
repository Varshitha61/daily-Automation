#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n+1);
    for (int i = 1; i <= n; ++i) cin >> a[i];
    unordered_map<int,int> freq;
    int bestB = -1, bestCnt = -1;
    for (int i = 1; i <= n; ++i) {
        int v = a[i] - (i-1)*k;
        if (v > 0) {
            int cnt = ++freq[v];
            if (cnt > bestCnt) {
                bestCnt = cnt;
                bestB = v;
            }
        }
    }
    if (bestB == -1) bestB = 1; // any positive value
    vector<tuple<char,int,int>> ops;
    for (int i = 1; i <= n; ++i) {
        int target = bestB + (i-1)*k;
        if (a[i] != target) {
            if (target > a[i]) ops.emplace_back('+', i, target - a[i]);
            else ops.emplace_back('-', i, a[i] - target);
        }
    }
    cout << ops.size() << "\n";
    for (auto &op : ops) {
        char c; int idx, d;
        tie(c, idx, d) = op;
        cout << c << ' ' << idx << ' ' << d << "\n";
    }
    return 0;
}