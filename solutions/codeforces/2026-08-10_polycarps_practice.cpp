#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 2005;

int n, k;
int a[MAXN];
int dp[MAXN][MAXN];
int pre[MAXN][MAXN];
int ans[MAXN];

int main() {
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    for (int i = 1; i <= n; i++) {
        dp[i][1] = *max_element(a + 1, a + i + 1);
    }

    for (int j = 2; j <= k; j++) {
        for (int i = j; i <= n; i++) {
            dp[i][j] = -1;
            for (int l = j - 1; l < i; l++) {
                int val = dp[l][j - 1] + *max_element(a + l + 1, a + i + 1);
                if (val > dp[i][j]) {
                    dp[i][j] = val;
                    pre[i][j] = l;
                }
            }
        }
    }

    cout << dp[n][k] << endl;

    int idx = n;
    for (int i = k; i >= 1; i--) {
        ans[i] = idx - pre[idx][i];
        idx = pre[idx][i];
    }

    for (int i = 1; i <= k; i++) {
        cout << ans[i] << " ";
    }

    return 0;
}