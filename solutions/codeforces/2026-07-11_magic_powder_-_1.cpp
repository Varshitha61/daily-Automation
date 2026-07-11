#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, k;
    cin >> n >> k;

    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }

    int l = 0, r = 1e6;
    while (l < r) {
        int m = (l + r + 1) / 2;
        long long sum = k;
        for (int i = 0; i < n; i++) {
            long long need = 1LL * m * a[i];
            if (b[i] < need) {
                sum -= need - b[i];
            }
        }
        if (sum >= 0) {
            l = m;
        } else {
            r = m - 1;
        }
    }

    cout << l << endl;

    return 0;
}