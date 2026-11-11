#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long A, B, X, Y;
        cin >> A >> B >> X >> Y;
        long long D = B - A;
        if (D == 0) {
            cout << "YES\n";
            continue;
        }
        long long low = max(0LL, -D);
        long long high = min(Y, X - D);
        if (low <= high) cout << "YES\n";
        else cout << "NO\n";
    }
    return 0;
}