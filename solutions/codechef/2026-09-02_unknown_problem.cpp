#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long H, X, Y;
        cin >> H >> X >> Y;
        long long without = (H + X - 1) / X;
        long long a = 0;
        if (H > Y) {
            a = (H - Y + X - 1) / X;
        }
        long long with = a + 1;
        cout << min(without, with) << "\n";
    }
    return 0;
}