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
        int scoreA_first = (500 - 2 * X) + (1000 - 4 * (X + Y));
        int scoreB_first = (1000 - 4 * Y) + (500 - 2 * (X + Y));
        cout << max(scoreA_first, scoreB_first) << "\n";
    }
    return 0;
}