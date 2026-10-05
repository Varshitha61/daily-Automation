#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n, I;
    if (!(cin >> n >> I)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());
    vector<long long> vals;
    vector<long long> cnt;
    for (int i = 0; i < n; ) {
        int j = i;
        while (j < n && a[j] == a[i]) ++j;
        vals.push_back(a[i]);
        cnt.push_back(j - i);
        i = j;
    }
    int m = vals.size();
    long long maxbits = (I * 8) / n; // floor
    long long Kmax;
    if (maxbits >= 30) {
        Kmax = m; // enough to keep all distinct values
    } else {
        Kmax = 1LL << maxbits;
        if (Kmax > m) Kmax = m;
    }
    if (Kmax >= m) {
        cout << 0 << "\n";
        return 0;
    }
    long long best = 0;
    long long cur = 0;
    int r = 0;
    for (int l = 0; l < m; ++l) {
        while (r < m && (r - l) < Kmax) {
            cur += cnt[r];
            ++r;
        }
        best = max(best, cur);
        cur -= cnt[l];
    }
    cout << (n - best) << "\n";
    return 0;
}