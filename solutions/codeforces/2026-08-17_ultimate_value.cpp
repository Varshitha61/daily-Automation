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
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        long long ans = 0;
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) {
                ans += a[i];
            } else {
                ans -= a[i];
            }
        }
        vector<int> b = a;
        sort(b.begin(), b.end());
        long long ans2 = 0;
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) {
                ans2 += b[i];
            } else {
                ans2 -= b[i];
            }
        }
        if (n % 2 == 0) {
            cout << max(ans, ans2) << endl;
        } else {
            cout << min(ans, ans2) << endl;
        }
    }
    return 0;
}