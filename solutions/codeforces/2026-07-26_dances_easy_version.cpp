#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n - 1), b(n);
        for (int i = 0; i < n - 1; i++) {
            cin >> a[i];
        }
        for (int i = 0; i < n; i++) {
            cin >> b[i];
        }
        a.insert(a.begin(), m);
        sort(a.begin(), a.end());
        sort(b.begin(), b.end());
        int ans = 0;
        int i = 0, j = 0;
        while (i < n && j < n) {
            if (a[i] >= b[j]) {
                ans++;
                j++;
            } else {
                i++;
                j++;
            }
        }
        cout << ans << endl;
    }
    return 0;
}