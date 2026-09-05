#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long A, B;
        cin >> A >> B;
        long long left = 2 * A;
        if (left > B) {
            cout << "FIRST";
        } else if (left < B) {
            cout << "SECOND";
        } else {
            cout << "ANY";
        }
        if (T) cout << '\n';
    }
    return 0;
}