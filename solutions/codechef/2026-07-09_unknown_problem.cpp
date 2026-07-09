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

    int ans = 0, diff = 0;
    for (int i = 0; i < n; i++) {
        if (a[i].first > a[i].second) {
            if (a[i].first - a[i].second > diff) {
                ans = i + 1;
                diff = a[i].first - a[i].second;
            }
        } else {
            if (a[i].second - a[i].first > diff) {
                ans = i + 1;
                diff = a[i].second - a[i].first;
            }
        }
    }

    cout << ans << " " << diff << endl;

    return 0;
}