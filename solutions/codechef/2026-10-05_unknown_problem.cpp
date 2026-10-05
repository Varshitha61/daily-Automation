#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long P, Q;
        cin >> P >> Q;
        long long sum = P + Q;
        long long block = (sum / 2) % 2;
        if (block == 0) cout << "Alice\n";
        else cout << "Bob\n";
    }
    return 0;
}