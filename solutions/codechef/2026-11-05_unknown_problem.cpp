#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long N, X;
        cin >> N >> X;
        long long deficit = max(0LL, N - X);
        long long packets = (deficit + 3) / 4; // ceil division by 4
        cout << packets;
        if (T) cout << '\n';
    }
    return 0;
}