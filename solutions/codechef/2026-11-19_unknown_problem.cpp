#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int A, B, C;
        cin >> A >> B >> C;
        int mx = max(A, max(B, C));
        cout << mx;
        if (T) cout << '\n';
    }
    return 0;
}