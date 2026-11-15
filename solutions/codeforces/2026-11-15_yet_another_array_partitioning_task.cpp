#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    vector<long long> b = a;
    sort(b.begin(), b.end(), greater<long long>());
    long long need = 1LL * m * k;
    long long sum = 0;
    for (int i = 0; i < need; ++i) sum += b[i];
    long long threshold = b[need - 1];
    long long cntGreater = 0;
    for (int i = 0; i < need; ++i) if (b[i] > threshold) ++cntGreater;
    long long needThresh = need - cntGreater; // how many equal to threshold we need
    vector<int> cuts;
    int curSel = 0, made = 0;
    for (int i = 0; i < n; ++i) {
        bool selected = false;
        if (a[i] > threshold) selected = true;
        else if (a[i] == threshold && needThresh > 0) {
            selected = true;
            --needThresh;
        }
        if (selected) {
            ++curSel;
            if (curSel == m && made < k - 1) {
                cuts.push_back(i + 1); // cut after position i (1-indexed)
                ++made;
                curSel = 0;
            }
        }
    }
    cout << sum << "\n";
    for (size_t i = 0; i < cuts.size(); ++i) {
        if (i) cout << ' ';
        cout << cuts[i];
    }
    if (!cuts.empty()) cout << "\n";
    return 0;
}