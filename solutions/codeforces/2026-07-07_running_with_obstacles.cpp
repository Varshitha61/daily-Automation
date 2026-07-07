#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m, s, d;
    cin >> n >> m >> s >> d;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    sort(a.begin(), a.end());
    int pos = 0;
    bool ok = true;
    vector<pair<string, int>> ans;
    while (pos < m) {
        int next_pos = pos + s;
        int next_obstacle = -1;
        for (int i = 0; i < n; i++) {
            if (a[i] > pos && a[i] <= next_pos) {
                next_obstacle = a[i];
                break;
            }
        }
        if (next_obstacle != -1) {
            int jump_len = min(d, next_obstacle + d - pos);
            int land_pos = next_obstacle + 1;
            ans.push_back({"RUN", next_obstacle - pos});
            ans.push_back({"JUMP", jump_len});
            pos = land_pos;
        } else {
            ans.push_back({"RUN", m - pos});
            pos = m;
        }
        if (pos < m && next_obstacle == m) {
            ok = false;
            break;
        }
    }
    if (!ok) {
        cout << "IMPOSSIBLE" << endl;
    } else {
        for (auto p : ans) {
            cout << p.first << " " << p.second << endl;
        }
    }
    return 0;
}