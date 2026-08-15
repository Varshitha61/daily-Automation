#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n), b(n);
        for (int i = 0; i < n; i++) cin >> a[i];
        for (int i = 0; i < n; i++) cin >> b[i];
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        int l = 0, r = 1e9 + 1;
        while (l < r) {
            int m = (l + r + 1) / 2;
            int cnt = 0;
            for (int i = 0; i < n; i++) {
                if (cnt < n && abs(a[i] - b[cnt]) >= m) cnt++;
            }
            if (cnt == n) l = m;
            else r = m - 1;
        }
        cout << l << endl;
    }
    return 0;
}