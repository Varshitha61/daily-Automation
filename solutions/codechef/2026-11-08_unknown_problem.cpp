#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long X, Y, D;
        cin >> X >> Y >> D;
        if (llabs(X - Y) <= D) cout << "YES\n";
        else cout << "NO\n";
    }
    return 0;
}