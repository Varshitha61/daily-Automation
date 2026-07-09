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
                        while (cnt[i] > 0 && cnt[j] > 0 && cnt[k] > 0) {
                            vector<int> tmp = {i, j, k};
                            ans.push_back(tmp);
                            cnt[i]--;
                            cnt[j]--;
                            cnt[k]--;
                        }
                    }
                }
            }
        }
    }

    if (ans.size() == n / 3) {
        for (int i = 0; i < ans.size(); i++) {
            for (int j = 0; j < ans[i].size(); j++) {
                cout << ans[i][j] << " ";
            }
            cout << endl;
        }
    } else {
        cout << -1 << endl;
    }

    return 0;
}