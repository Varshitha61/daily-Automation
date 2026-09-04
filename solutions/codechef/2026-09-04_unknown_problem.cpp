#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int b1, b2, b3;
        cin >> b1 >> b2 >> b3;
        int empty = (b1 == 0) + (b2 == 0) + (b3 == 0);
        if (empty >= 2)
            cout << "Water filling time";
        else
            cout << "Not now";
        if (T) cout << '\n';
    }
    return 0;
}