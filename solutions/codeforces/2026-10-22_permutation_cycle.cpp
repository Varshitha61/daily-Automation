#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long N, A, B;
    if (!(cin >> N >> A >> B)) return 0;
    long long x = -1, y = -1;
    if (A == B) {
        if (N % A != 0) {
            cout << -1 << "\n";
            return 0;
        }
        x = N / A;
        y = 0;
    } else {
        for (long long i = 0; i * A <= N; ++i) {
            long long rem = N - i * A;
            if (rem % B == 0) {
                x = i;
                y = rem / B;
                break;
            }
        }
        if (x == -1) {
            cout << -1 << "\n";
            return 0;
        }
    }
    vector<int> perm(N + 1);
    long long cur = 1;
    for (long long i = 0; i < x; ++i) {
        for (long long j = 0; j < A - 1; ++j) perm[cur + j] = cur + j + 1;
        perm[cur + A - 1] = cur;
        cur += A;
    }
    for (long long i = 0; i < y; ++i) {
        for (long long j = 0; j < B - 1; ++j) perm[cur + j] = cur + j + 1;
        perm[cur + B - 1] = cur;
        cur += B;
    }
    for (long long i = 1; i <= N; ++i) {
        if (i > 1) cout << ' ';
        cout << perm[i];
    }
    cout << "\n";
    return 0;
}