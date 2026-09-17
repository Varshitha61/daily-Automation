#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int X, Y;
        cin >> X >> Y;
        if (X < Y) cout << "BIKE";
        else if (Y < X) cout << "CAR";
        else cout << "SAME";
        if (T) cout << '\n';
    }
    return 0;
}