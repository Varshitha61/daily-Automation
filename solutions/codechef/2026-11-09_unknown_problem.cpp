#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int N, X;
        cin >> N >> X;
        int ans = min(X, N - X);
        cout << ans;
        if (T) cout << '\n';
    }
    return 0;
}