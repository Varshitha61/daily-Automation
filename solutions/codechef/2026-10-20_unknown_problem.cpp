#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long W, X, Y, Z;
        cin >> W >> X >> Y >> Z;
        bool ok = false;
        if (W == X || W == Y || W == Z) ok = true;
        else if (W == X + Y || W == X + Z || W == Y + Z) ok = true;
        else if (W == X + Y + Z) ok = true;
        cout << (ok ? "YES" : "NO") << '\n';
    }
    return 0;
}