#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int t;
    cin >> t;

    while (t--) {
        int n, k;
        cin >> n >> k;

        vector<int> a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }

        sort(a.begin(), a.end());

        long long ans = 0;
        for (int i = 0; i < n; i++) {
            ans += a[i] / k;
        }

        vector<int> b;
        for (int i = 0; i < n; i++) {
            b.push_back(a[i] % k);
        }

        sort(b.begin(), b.end());

        int l = 0, r = n - 1;
        while (l < r) {
            if (b[l] + b[r] >= k) {
                ans++;
                l++;
                r--;
            } else {
                l++;
            }
        }

        cout << ans << endl;
    }

    return 0;
}