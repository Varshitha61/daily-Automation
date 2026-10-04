#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    const int MAXN = 100000;
    vector<bool> isPrime(MAXN + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= MAXN; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= MAXN; j += i)
                isPrime[j] = false;
        }
    }
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        long long N;
        cin >> N;
        if (N >= 0 && N <= MAXN && isPrime[(int)N])
            cout << "yes\n";
        else
            cout << "no\n";
    }
    return 0;
}