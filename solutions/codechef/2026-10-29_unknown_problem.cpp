#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long X;
        cin >> X;
        int r = X % 3;
        if (r == 0) cout << "NORMAL";
        else if (r == 1) cout << "HUGE";
        else cout << "SMALL";
        if (T) cout << '\n';
    }
    return 0;
}