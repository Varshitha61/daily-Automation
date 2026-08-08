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
                diff = a[i].first - a[i].second;
                ans = i + 1;
            }
        }
    }

    cout << ans << " " << diff << endl;

    return 0;
}