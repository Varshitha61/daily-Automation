#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, k;
        cin >> n >> k;

        vector<int> a(n), b(n);
        for (int i = 0; i < n; ++i) {
            cin >> a[i];
        }
        for (int i = 0; i < n; ++i) {
            cin >> b[i];
        }

        vector<int> c(n);
        for (int i = 0; i < n; ++i) {
            c[i] = abs(a[i] - b[i]);
        }

        sort(c.begin(), c.end());

        long long ans = 0;
        for (int i = 0; i < n - 2 * k; ++i) {
            ans += c[i];
        }

        for (int i = n - 2 * k; i < n - k; ++i) {
            ans += 2 * c[i];
        }

        for (int i = n - k; i < n; ++i) {
            ans += 3 * c[i];
        }

        cout << ans << '\n';
    }

    return 0;
}