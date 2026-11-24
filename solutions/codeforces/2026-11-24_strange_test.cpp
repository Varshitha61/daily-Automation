#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int a, b;
        cin >> a >> b;
        int ans = b - a; // only increment a
        for (int B = b; B <= 2 * b; ++B) {
            if ((a | B) == B) {
                ans = min(ans, B - b + 1);
                break;
            }
        }
        cout << ans << "\n";
    }
    return 0;
}