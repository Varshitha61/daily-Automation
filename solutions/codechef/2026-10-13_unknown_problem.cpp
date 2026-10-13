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
        int ones = 0;
        for (int i = 0; i < N; ++i) {
            int x; cin >> x;
            if (x == 1) ++ones;
        }
        if (N % 2 == 1) {
            cout << -1 << "\n";
        } else {
            cout << abs(ones - N/2) << "\n";
        }
    }
    return 0;
}