#include <bits/stdc++.h>
using namespace std;

const int MAXN = 2e5 + 5;

vector<int> adj[MAXN];
int deg[MAXN];

void solve() {
    int n, m;
    cin >> n >> m;

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
        deg[u]++;
        deg[v]++;
    }

    if (m != n - 1) {
        cout << "No" << endl;
        return;
    }

    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (deg[i] > 2) {
            cout << "No" << endl;
            return;
        }
        if (deg[i] == 1) {
            cnt++;
        }
    }

    if (cnt != 2) {
        cout << "No" << endl;
        return;
    }

    vector<bool> vis(n, false);
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (deg[i] == 1) {
            q.push(i);
            vis[i] = true;
            break;
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (!vis[v]) {
                vis[v] = true;
                q.push(v);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            cout << "No" << endl;
            return;
        }
    }

    cout << "Yes" << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();

    return 0;
}