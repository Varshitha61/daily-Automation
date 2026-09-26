#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long A, B, C;
        cin >> A >> B >> C;
        long long sum = A + B + C;
        long long mn = min({A, B, C});
        cout << sum - mn;
        if (T) cout << '\n';
    }
    return 0;
}