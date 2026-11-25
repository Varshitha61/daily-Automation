#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int MAXA = 1000000;
    vector<int> spf(MAXA + 1);
    for (int i = 2; i <= MAXA; ++i) {
        if (!spf[i]) {
            for (int j = i; j <= MAXA; j += i)
                if (!spf[j]) spf[j] = i;
        }
    }
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n + 1);
        for (int i = 1; i <= n; ++i) cin >> a[i];
        vector<vector<int>> g(n + 1);
        for (int i = 0; i < n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        // compute square-free part parity (whether equals 1)
        vector<char> goodRoot(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            int x = a[i];
            int res = 1;
            while (x > 1) {
                int p = spf[x];
                int cnt = 0;
                while (x % p == 0) {
                    x /= p;
                    cnt ^= 1; // only parity needed
                }
                if (cnt) {
                    if ((long long)res * p > MAXA) { // overflow not possible within limits
                        res = MAXA + 1;
                    } else res *= p;
                }
            }
            if (res == 1) goodRoot[i] = 1;
        }
        vector<int> parent(n + 1, 0), order;
        order.reserve(n);
        parent[1] = -1;
        order.push_back(1);
        for (size_t idx = 0; idx < order.size(); ++idx) {
            int v = order[idx];
            for (int to : g[v]) if (to != parent[v]) {
                parent[to] = v;
                order.push_back(to);
            }
        }
        vector<int> sub(n + 1, 1);
        for (int i = n - 1; i >= 0; --i) {
            int v = order[i];
            for (int to : g[v]) if (to != parent[v]) {
                sub[v] += sub[to];
            }
        }
        long long answer = 0;
        for (int v = 1; v <= n; ++v) {
            if (!goodRoot[v]) continue;
            long long S = 0, Q = 0, R = 0;
            for (int to : g[v]) {
                long long sz = (to == parent[v]) ? (n - sub[v]) : sub[to];
                S += sz;
                Q += sz * sz;
                R += sz * sz * sz;
            }
            if (S < 3) continue;
            __int128 tmp = (__int128)S * S * S - (__int128)3 * S * Q + (__int128)2 * R;
            long long contrib = (long long)(tmp / 6);
            answer += contrib;
        }
        cout << answer << "\n";
    }
    return 0;
}