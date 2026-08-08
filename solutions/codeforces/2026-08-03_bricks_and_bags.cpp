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
        sort(a.begin(), a.end());
        int ans = 0;
        for (int i = 0; i < n - 2; i++) {
            ans = max(ans, a[n - 1] + a[i] - 2 * a[i + 1]);
        }
        for (int i = 2; i < n; i++) {
            ans = max(ans, 2 * a[i - 1] - a[0] - a[i]);
        }
        cout << ans << endl;
    }
    return 0;
}