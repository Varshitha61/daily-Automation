#include <bits/stdc++.h>
using namespace std;

bool canWin(const vector<int>& a, int k) {
    multiset<int> ms(a.begin(), a.end());
    for (int t = k; t >= 1; --t) {
        // Alice deletes the largest element <= t
        auto it = ms.upper_bound(t);
        if (it == ms.begin()) return false; // none <= t
        --it;
        ms.erase(it);
        // Bob's move
        if (!ms.empty()) {
            auto it2 = ms.begin(); // smallest element
            int val = *it2;
            ms.erase(it2);
            ms.insert(val + t);
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int &x : a) cin >> x;
        sort(a.begin(), a.end());
        int ans = 0;
        for (int k = n; k >= 0; --k) {
            if (canWin(a, k)) {
                ans = k;
                break;
            }
        }
        cout << ans << "\n";
    }
    return 0;
}