#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long X, Y, Z;
        cin >> X >> Y >> Z;
        long long breaks = (X - 1) / 3;
        long long total = X * Y + breaks * Z;
        cout << total;
        if (T) cout << '\n';
    }
    return 0;
}