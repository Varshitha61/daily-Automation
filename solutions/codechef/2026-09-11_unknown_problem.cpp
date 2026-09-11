#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long a,b,c,d;
        cin >> a >> b >> c >> d;
        long long ans = max(a,b) + max(c,d);
        cout << ans;
        if (T) cout << '\n';
    }
    return 0;
}