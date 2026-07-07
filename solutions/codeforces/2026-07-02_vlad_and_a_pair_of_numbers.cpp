#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        long long x;
        cin >> x;

        long long a = x;
        long long b = 0;

        for (int i = 0; i < 32; i++) {
            if ((x & (1LL << i)) != 0) {
                continue;
            }
            if ((a & (1LL << i)) == 0) {
                a += (1LL << i);
                b += (1LL << i);
            }
        }

        if ((a + b) == 2 * x && (a ^ b) == x) {
            cout << a << " " << b << endl;
        } else {
            cout << -1 << endl;
        }
    }

    return 0;
}