#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int X, Y, A, B;
        cin >> X >> Y >> A >> B;
        int ans = 0;
        if (X != A && X != B) ++ans;
        if (Y != A && Y != B) ++ans;
        cout << ans << '\n';
    }
    return 0;
}