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
        vector<long long> a(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        sort(a.begin(), a.end());
        vector<long long> diffs;
        long long prev = 0;
        for (long long x : a) {
            long long d = x - prev;
            if (d > 0) diffs.push_back(d);
            prev = x;
        }
        int idx = -1;
        for (int i = 0; i < (int)diffs.size(); ++i) {
            if (diffs[i] > 1) {
                idx = i;
                break;
            }
        }
        bool alice;
        if (idx == -1) {
            alice = (diffs.size() % 2 == 1);
        } else {
            alice = (idx % 2 == 0);
        }
        cout << (alice ? "Alice" : "Bob") << '\n';
    }
    return 0;
}