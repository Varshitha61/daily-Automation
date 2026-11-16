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
        long long subs = (N + 5) / 6; // ceil division
        cout << subs * X << "\n";
    }
    return 0;
}