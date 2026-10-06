#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long N, X, K;
        cin >> N >> X >> K;
        long long maxFill = K / X;
        long long ans = min(N, maxFill);
        cout << ans << '\n';
    }
    return 0;
}