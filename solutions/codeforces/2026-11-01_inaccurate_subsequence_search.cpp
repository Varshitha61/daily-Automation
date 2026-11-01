#include <bits/stdc++.h>
using namespace std;

const int MAXV = 1000000 + 5;
static int cntB[MAXV];
static int cntW[MAXV];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n, m, k;
        cin >> n >> m >> k;
        vector<int> a(n);
        for (int &x : a) cin >> x;
        vector<int> b(m);
        for (int &x : b) cin >> x;
        vector<int> usedB;
        usedB.reserve(m);
        for (int x : b) {
            if (cntB[x] == 0) usedB.push_back(x);
            ++cntB[x];
        }
        vector<int> usedW;
        usedW.reserve(m);
        long long inter = 0;
        auto add = [&](int val) {
            int prev = cntW[val];
            if (prev == 0) usedW.push_back(val);
            cntW[val] = prev + 1;
            if (prev < cntB[val]) ++inter;
        };
        auto remove = [&](int val) {
            int prev = cntW[val];
            cntW[val] = prev - 1;
            if (prev <= cntB[val]) --inter;
        };
        for (int i = 0; i < m; ++i) add(a[i]);
        long long ans = (inter >= k);
        for (int i = m; i < n; ++i) {
            remove(a[i - m]);
            add(a[i]);
            if (inter >= k) ++ans;
        }
        cout << ans << '\n';
        // reset
        for (int x : usedB) cntB[x] = 0;
        for (int x : usedW) cntW[x] = 0;
    }
    return 0;
}