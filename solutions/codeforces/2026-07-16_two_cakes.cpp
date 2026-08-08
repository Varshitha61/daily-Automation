#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> a(2 * n);
    for (int i = 0; i < 2 * n; i++) {
        cin >> a[i];
    }

    vector<vector<int>> pos(n + 1, vector<int>(2));
    for (int i = 0; i < 2 * n; i++) {
        if (pos[a[i]][0] == 0) {
            pos[a[i]][0] = i + 1;
        } else {
            pos[a[i]][1] = i + 1;
        }
    }

    int ans = 0;
    int l = 0, r = 2 * n + 1;
    for (int i = 1; i <= n; i++) {
        if (pos[i][0] < pos[i][1]) {
            ans += pos[i][0] - l;
            l = pos[i][0];
            ans += r - pos[i][1];
            r = pos[i][1];
        } else {
            ans += r - pos[i][0];
            r = pos[i][0];
            ans += pos[i][1] - l;
            l = pos[i][1];
        }
    }

    cout << ans << endl;

    return 0;
}