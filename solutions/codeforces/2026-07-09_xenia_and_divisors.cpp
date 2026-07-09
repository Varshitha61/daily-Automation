#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    vector<int> cnt(8, 0);
    for (int i = 0; i < n; i++) {
        cnt[a[i]]++;
    }

    vector<vector<int>> ans;
    for (int i = 0; i < n / 3; i++) {
        bool found = false;
        for (int x = 1; x <= 7; x++) {
            if (cnt[x] > 0) {
                for (int y = x + 1; y <= 7; y++) {
                    if (cnt[y] > 0 && y % x == 0) {
                        for (int z = y + 1; z <= 7; z++) {
                            if (cnt[z] > 0 && z % y == 0) {
                                ans.push_back({x, y, z});
                                cnt[x]--;
                                cnt[y]--;
                                cnt[z]--;
                                found = true;
                                break;
                            }
                        }
                        if (found) break;
                    }
                }
                if (found) break;
            }
        }
        if (!found) {
            cout << -1 << endl;
            return 0;
        }
    }

    for (int i = 0; i < ans.size(); i++) {
        cout << ans[i][0] << " " << ans[i][1] << " " << ans[i][2];
        if (i < ans.size() - 1) cout << endl;
    }

    return 0;
}