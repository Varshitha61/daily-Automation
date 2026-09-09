#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long X, N;
        cin >> X >> N;
        long long required = (N + 99) / 100; // ceil division
        long long ans = required > X ? required - X : 0;
        cout << ans;
        if (T) cout << '\n';
    }
    return 0;
}