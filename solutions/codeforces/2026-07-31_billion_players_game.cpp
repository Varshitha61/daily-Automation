#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n, l, r;
        cin >> n >> l >> r;
        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        sort(a.begin(), a.end());
        int ans = 0;
        for (int i = 0; i < n; i++) {
            if (a[i] < l) {
                ans = max(ans, l - a[i]);
            } else if (a[i] > r) {
                ans = max(ans, a[i] - r);
            }
        }
        int sum = 0;
        for (int i = 0; i < n; i++) {
            if (a[i] < l) {
                sum += l - a[i];
            } else if (a[i] > r) {
                sum += a[i] - r;
            }
        }
        cout << min(ans, sum) << endl;
    }
    return 0;
}