#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    unordered_map<int, vector<int>> pos;
    pos.reserve(n * 2);
    for (int i = 1; i <= n; ++i) {
        int x;
        cin >> x;
        pos[x].push_back(i);
    }
    int maxFreq = 0;
    for (auto &p : pos) {
        int sz = p.second.size();
        if (sz > maxFreq) maxFreq = sz;
    }
    int bestLen = n + 1, ansL = 1, ansR = n;
    for (auto &p : pos) {
        const vector<int> &v = p.second;
        if ((int)v.size() == maxFreq) {
            for (int i = 0; i + maxFreq - 1 < (int)v.size(); ++i) {
                int l = v[i];
                int r = v[i + maxFreq - 1];
                int len = r - l + 1;
                if (len < bestLen) {
                    bestLen = len;
                    ansL = l;
                    ansR = r;
                }
            }
        }
    }
    cout << ansL << ' ' << ansR << '\n';
    return 0;
}