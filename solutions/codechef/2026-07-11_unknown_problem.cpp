#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<pair<int, int>> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i].first >> a[i].second;
    }

    int ans = 0;
    int diff = 0;
    for (int i = 0; i < n; i++) {
        int sum = a[i].first + a[i].second;
        if (sum % 2 != 0) {
            ans++;
            diff = sum / 2;
        }
    }

    if (ans == 1) {
        cout << ans << " " << diff << endl;
    } else {
        cout << 0 << endl;
    }

    return 0;
}