#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long X, Y;
        cin >> X >> Y;
        cout << llabs(X - Y) << "\n";
    }
    return 0;
}