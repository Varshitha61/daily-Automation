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
        long long a = X / Y;
        long long b = X % Y;
        cout << a + b;
        if (T) cout << '\n';
    }
    return 0;
}