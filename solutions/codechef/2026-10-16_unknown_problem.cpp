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
        long long travel = Y / X;
        long long D = Z - travel;
        if (D < 0) D = 0;
        cout << D << "\n";
    }
    return 0;
}