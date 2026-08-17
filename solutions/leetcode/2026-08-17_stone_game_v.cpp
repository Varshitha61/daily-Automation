#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int stoneGameV(vector<int>& stoneValue) {
        int n = stoneValue.size();
        vector<int> prefix(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefix[i + 1] = prefix[i] + stoneValue[i];
        }
        vector<vector<int>> dp(n, vector<int>(n, 0));
        for (int length = 1; length < n; length++) {
            for (int i = 0; i + length < n; i++) {
                int j = i + length;
                for (int k = i; k < j; k++) {
                    int left = prefix[k + 1] - prefix[i];
                    int right = prefix[j + 1] - prefix[k + 1];
                    if (left < right) {
                        dp[i][j] = max(dp[i][j], dp[i][k] + left);
                    } else if (left > right) {
                        dp[i][j] = max(dp[i][j], dp[k + 1][j] + right);
                    } else {
                        dp[i][j] = max(dp[i][j], max(dp[i][k], dp[k + 1][j]) + left);
                    }
                }
            }
        }
        return dp[0][n - 1];
    }
};

int main() {
    Solution solution;
    vector<int> stoneValue = {6, 2, 3, 4, 5, 5};
    cout << solution.stoneGameV(stoneValue) << endl;
    stoneValue = {7, 7, 7, 7, 7, 7, 7};
    cout << solution.stoneGameV(stoneValue) << endl;
    stoneValue = {4};
    cout << solution.stoneGameV(stoneValue) << endl;
    return 0;
}