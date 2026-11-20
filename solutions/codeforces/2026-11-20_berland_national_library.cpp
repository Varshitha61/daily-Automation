#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    unordered_set<int> inside;
    int cur = 0, ans = 0;
    for (int i = 0; i < n; ++i) {
        char c;
        int id;
        cin >> c >> id;
        if (c == '+') {
            cur++;
            ans = max(ans, cur);
            inside.insert(id);
        } else { // '-'
            if (inside.count(id)) {
                cur--;
                inside.erase(id);
            } else {
                // person was inside before logging started
                ans++;
                // cur unchanged
            }
        }
    }
    cout << ans << "\n";
    return 0;
}