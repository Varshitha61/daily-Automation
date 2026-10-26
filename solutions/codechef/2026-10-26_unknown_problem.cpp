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
        long long need = A * B;
        long long have = X * Y;
        if (have >= need) cout << "Yes\n";
        else cout << "No\n";
    }
    return 0;
}