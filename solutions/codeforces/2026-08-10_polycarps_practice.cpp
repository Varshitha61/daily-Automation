#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    int n, k;
    cin >> n >> k;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    vector<vector<int>> dp(n + 1, vector<int>(k + 1, INT_MIN));
    vector<vector<int>> prev(n + 1, vector<int>(k + 1, -1));

    dp[0][0] = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= min(i, k); j++) {
            int max_val = 0;
            for (int l = i - 1; l >= 0; l--) {
                max_val = max(max_val, a[l]);
                if (dp[l][j - 1] != INT_MIN && dp[l][j - 1] + max_val > dp[i][j]) {
                    dp[i][j] = dp[l][j - 1] + max_val;
                    prev[i][j] = l;
                }
            }
        }
    }

    cout << dp[n][k] << endl;

    vector<int> ans;
    int i = n, j = k;
    while (j > 0) {
        ans.push_back(i - prev[i][j]);
        i = prev[i][j];
        j--;
    }

    reverse(ans.begin(), ans.end());

    for (int x : ans) {
        cout << x << " ";
    }

    return 0;
}