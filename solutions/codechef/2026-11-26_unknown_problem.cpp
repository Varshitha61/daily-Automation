#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long N;
        cin >> N;
        long long ans = 0;
        if (N >= 2) ans = (N - 2) / 7 + 1;
        cout << ans;
        if (T) cout << '\n';
    }
    return 0;
}