#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minPositiveSumSubarray(vector<int>& nums, int l, int r) {
        int n = nums.size();
        vector<int> pref(n + 1, 0);
        for (int i = 0; i < n; ++i) pref[i + 1] = pref[i] + nums[i];
        int ans = INT_MAX;
        for (int i = 0; i < n; ++i) {
            for (int len = l; len <= r; ++len) {
                int j = i + len - 1;
                if (j >= n) break;
                int sum = pref[j + 1] - pref[i];
                if (sum > 0) ans = min(ans, sum);
            }
        }
        return ans == INT_MAX ? -1 : ans;
    }
};