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
        long long left = A * Y;
        long long right = B * X;
        if (left < right) {
            cout << "Chef";
        } else if (left > right) {
            cout << "Chefina";
        } else {
            cout << "Both";
        }
        if (T) cout << '\n';
    }
    return 0;
}