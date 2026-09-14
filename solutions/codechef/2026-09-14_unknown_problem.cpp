#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long A, B, C;
        cin >> A >> B >> C;
        if (A + B > 2 * C) cout << "YES\n";
        else cout << "NO\n";
    }
    return 0;
}