#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    if (n == 1) {
        cout << a[0] << "\n";
        return 0;
    }
    vector<long long> pref(n), suff(n);
    for (int i = 0; i < n; ++i) {
        pref[i] = (i == 0 ? a[i] : (pref[i-1] | a[i]));
    }
    for (int i = n-1; i >= 0; --i) {
        suff[i] = (i == n-1 ? a[i] : (suff[i+1] | a[i]));
    }
    long long bestVal = -1;
    int bestIdx = 0;
    for (int i = 0; i < n; ++i) {
        long long orRest = 0;
        if (i > 0) orRest |= pref[i-1];
        if (i + 1 < n) orRest |= suff[i+1];
        long long val = a[i] & (~orRest);
        if (val > bestVal) {
            bestVal = val;
            bestIdx = i;
        }
    }
    cout << a[bestIdx];
    for (int i = 0; i < n; ++i) {
        if (i == bestIdx) continue;
        cout << ' ' << a[i];
    }
    cout << "\n";
    return 0;
}