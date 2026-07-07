#include <iostream>
#include <vector>
#include <algorithm>

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

    for (int i = 0; i < n; i++) {
        if (pos < a[i]) {
            if (a[i] - pos >= s) {
                ans.push_back({"RUN", s});
                pos += s;
                if (pos < a[i]) {
                    ans.push_back({"JUMP", min(d, a[i] - pos)});
                    pos = a[i] + 1;
                }
            } else {
                ok = false;
                break;
            }
        } else {
            if (i + 1 < n && a[i + 1] - pos <= d) {
                continue;
            } else {
                ans.push_back({"JUMP", min(d, m - pos)});
                pos = min(m, pos + d);
            }
        }
    }

    if (pos < m) {
        ans.push_back({"RUN", m - pos});
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