#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    if (n < 3 || m != n) {
        cout << "NO\n";
        return 0;
    }
    vector<int> visited(n + 1, 0);
    queue<int> q;
    q.push(1);
    visited[1] = 1;
    int cnt = 0;
    while (!q.empty()) {
        int v = q.front(); q.pop();
        ++cnt;
        for (int to : adj[v]) {
            if (!visited[to]) {
                visited[to] = 1;
                q.push(to);
            }
        }
    }
    if (cnt == n) cout << "FHTAGN!\n";
    else cout << "NO\n";
    return 0;
}