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
    vector<string> ans;
    while (pos < m) {
        int run = s;
        while (run > 0 && pos < m) {
            if (binary_search(a.begin(), a.end(), pos)) {
                ok = false;
                break;
            }
            pos++;
            run--;
        }
        if (!ok) break;
        ans.push_back("RUN " + to_string(s));
        if (pos >= m) break;
        int jump = d;
        while (jump > 0 && pos < m) {
            if (binary_search(a.begin(), a.end(), pos)) {
                pos++;
                jump--;
            } else {
                break;
            }
        }
        ans.push_back("JUMP " + to_string(d - jump));
        pos += jump;
    }
    if (pos >= m) {
        for (auto& x : ans) {
            cout << x << endl;
        }
    } else {
        cout << "IMPOSSIBLE" << endl;
    }
    return 0;
}