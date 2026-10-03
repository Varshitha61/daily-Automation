#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long S, X, Y, Z;
        cin >> S >> X >> Y >> Z;
        long long free_mem = S - X - Y;
        if (free_mem >= Z) {
            cout << 0 << "\n";
        } else if (free_mem + Y >= Z) {
            cout << 1 << "\n";
        } else {
            cout << 2 << "\n";
        }
    }
    return 0;
}