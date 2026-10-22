#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long N, A, B;
        cin >> N >> A >> B;
        int rounds = 0;
        while (N > 1) {
            N >>= 1;
            ++rounds;
        }
        long long total = 1LL * rounds * A + 1LL * (rounds - 1) * B;
        cout << total << '\n';
    }
    return 0;
}