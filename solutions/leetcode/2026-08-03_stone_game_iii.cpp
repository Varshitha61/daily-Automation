#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

string stoneGameIII(vector<int>& stoneValue) {
    int n = stoneValue.size();
    vector<int> suffixSum(n + 1, 0);
    for (int i = n - 1; i >= 0; --i) {
        suffixSum[i] = suffixSum[i + 1] + stoneValue[i];
    }
    vector<int> dp(n + 1, INT_MIN);
    dp[n] = 0;
    for (int i = n - 1; i >= 0; --i) {
        for (int x = 1; x <= 3; ++x) {
            if (i + x > n) break;
            dp[i] = max(dp[i], suffixSum[i] - dp[i + x]);
        }
    }
    if (dp[0] == suffixSum[0] / 2) return "Tie";
    else if (dp[0] > suffixSum[0] / 2) return "Alice";
    else return "Bob";
}

int main() {
    vector<int> stoneValue = {1, 2, 3, 7};
    cout << stoneGameIII(stoneValue) << endl;
    stoneValue = {1, 2, 3, -9};
    cout << stoneGameIII(stoneValue) << endl;
    stoneValue = {1, 2, 3, 6};
    cout << stoneGameIII(stoneValue) << endl;
    return 0;
}