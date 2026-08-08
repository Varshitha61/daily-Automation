#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

const int MOD = 1e9 + 7;

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int count(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(1 << n, 0);
    for (int mask = 1; mask < (1 << n); mask++) {
        vector<int> sub;
        for (int i = 0; i < n; i++) {
            if ((mask >> i) & 1) sub.push_back(nums[i]);
        }
        int g = sub[0];
        for (int i = 1; i < sub.size(); i++) {
            g = gcd(g, sub[i]);
        }
        dp[mask] = 1;
        for (int submask = mask; submask > 0; submask = (submask - 1) & mask) {
            if (submask == mask) continue;
            int sub_g = dp[submask];
            int rest_g = dp[mask ^ submask];
            if (gcd(sub_g, rest_g) == g) {
                dp[mask] = (dp[mask] + dp[submask] * dp[mask ^ submask]) % MOD;
            }
        }
    }
    return dp[(1 << n) - 1];
}

int numberOfSubsequences(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(1 << n, 0);
    for (int mask = 1; mask < (1 << n); mask++) {
        vector<int> sub;
        for (int i = 0; i < n; i++) {
            if ((mask >> i) & 1) sub.push_back(nums[i]);
        }
        int g = sub[0];
        for (int i = 1; i < sub.size(); i++) {
            g = gcd(g, sub[i]);
        }
        dp[mask] = 1;
        for (int submask = mask; submask > 0; submask = (submask - 1) & mask) {
            if (submask == mask) continue;
            int sub_g = dp[submask];
            int rest_g = dp[mask ^ submask];
            if (gcd(sub_g, rest_g) == g) {
                dp[mask] = (dp[mask] + dp[submask] * dp[mask ^ submask]) % MOD;
            }
        }
    }
    int ans = 0;
    for (int mask = 1; mask < (1 << n); mask++) {
        for (int submask = mask; submask > 0; submask = (submask - 1) & mask) {
            if (submask == mask) continue;
            int sub_g = dp[submask];
            int rest_g = dp[mask ^ submask];
            if (gcd(sub_g, rest_g) == gcd(dp[mask])) {
                ans = (ans + dp[submask] * dp[mask ^ submask]) % MOD;
            }
        }
    }
    return ans;
}

int main() {
    vector<int> nums = {1, 2, 3, 4};
    cout << numberOfSubsequences(nums) << endl;
    return 0;
}