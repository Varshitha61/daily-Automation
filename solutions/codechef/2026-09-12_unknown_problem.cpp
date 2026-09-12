#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long X, A, B;
        cin >> X >> A >> B;
        long long score = A + 2 * B;
        if (score >= X) cout << "Qualify";
        else cout << "NotQualify";
        if (T) cout << '\n';
    }
    return 0;
}