#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int N;
        cin >> N;
        int p = 0, m = 0;
        for (int i = 0; i < N; ++i) {
            int x; cin >> x;
            if (x == 1) ++p;
            else --m; // x == -1
        }
        m = -m; // convert to count of -1
        int S = p - m;
        if (S % 2 != 0) {
            cout << -1 << "\n";
            continue;
        }
        int d = S / 2; // x - y = d
        int L = max(0, -d);
        int U = min(m, p - d);
        if (L > U) {
            cout << -1 << "\n";
        } else {
            int ans = 2 * L + d;
            cout << ans << "\n";
        }
    }
    return 0;
}