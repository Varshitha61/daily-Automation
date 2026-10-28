#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long N, M;
        cin >> N >> M;
        long long ans = N + max(0LL, N - M);
        cout << ans;
        if (T) cout << '\n';
    }
    return 0;
}