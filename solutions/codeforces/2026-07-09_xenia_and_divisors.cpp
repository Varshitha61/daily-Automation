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
    for (int i = 1; i <= 7; i++) {
        for (int j = i + 1; j <= 7; j++) {
            if (j % i == 0) {
                for (int k = j + 1; k <= 7; k++) {
                    if (k % j == 0) {
                        int min_cnt = min(min(cnt[i], cnt[j]), cnt[k]);
                        for (int t = 0; t < min_cnt; t++) {
                            ans.push_back({i, j, k});
                        }
                        cnt[i] -= min_cnt;
                        cnt[j] -= min_cnt;
                        cnt[k] -= min_cnt;
                    }
                }
            }
        }
    }
    if (ans.size() * 3 != n) {
        cout << -1 << endl;
    } else {
        for (int i = 0; i < ans.size(); i++) {
            for (int j = 0; j < 3; j++) {
                cout << ans[i][j] << " ";
            }
            cout << endl;
        }
    }
    return 0;
}