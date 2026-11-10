#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<int>> A(n, vector<int>(m));
    vector<vector<int>> B(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) cin >> A[i][j];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) cin >> B[i][j];
    vector<vector<int>> D(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) D[i][j] = A[i][j] ^ B[i][j];
    if (n == 1 || m == 1) {
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                if (D[i][j]) { cout << "No\n"; return 0; }
        cout << "Yes\n";
        return 0;
    }
    for (int i = 0; i < n; ++i) {
        int xr = 0;
        for (int j = 0; j < m; ++j) xr ^= D[i][j];
        if (xr) { cout << "No\n"; return 0; }
    }
    for (int j = 0; j < m; ++j) {
        int xr = 0;
        for (int i = 0; i < n; ++i) xr ^= D[i][j];
        if (xr) { cout << "No\n"; return 0; }
    }
    cout << "Yes\n";
    return 0;
}