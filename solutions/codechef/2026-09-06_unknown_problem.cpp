#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long N, K, M;
        cin >> N >> K >> M;
        long long cap = K * M;
        long long bags = (N + cap - 1) / cap;
        cout << bags;
        if (T) cout << '\n';
    }
    return 0;
}