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
        int x = a[i].first;
        int y = a[i].second;
        if (x > y) {
            swap(x, y);
        }
        if (y - x > diff) {
            diff = y - x;
            ans = 1;
        } else if (y - x == diff) {
            ans++;
        }
    }

    cout << ans << " " << diff << endl;

    return 0;
}