#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        int l = 0, r = n - 1;
        long long sumA = 0, sumB = 0;
        long long prev = 0;
        int moves = 0;
        bool aliceTurn = true;
        while (l <= r) {
            ++moves;
            long long cur = 0;
            if (aliceTurn) {
                while (l <= r && cur <= prev) {
                    cur += a[l];
                    ++l;
                }
                sumA += cur;
            } else {
                while (l <= r && cur <= prev) {
                    cur += a[r];
                    --r;
                }
                sumB += cur;
            }
            prev = cur;
            aliceTurn = !aliceTurn;
        }
        cout << moves << ' ' << sumA << ' ' << sumB << "\n";
    }
    return 0;
}