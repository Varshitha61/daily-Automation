#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long n, k;
        cin >> n >> k; // k is always 3
        if (n % 2 == 1) {
            long long a = 1;
            long long b = (n - 1) / 2;
            cout << a << ' ' << b << ' ' << b << "\n";
        } else {
            if (n % 4 == 0) {
                long long a = n / 2;
                long long b = n / 4;
                cout << a << ' ' << b << ' ' << b << "\n";
            } else {
                long long a = 2;
                long long b = (n - 2) / 2;
                cout << a << ' ' << b << ' ' << b << "\n";
            }
        }
    }
    return 0;
}