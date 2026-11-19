#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minScore(vector<int>& nums) {
        int n = nums.size();
        sort(nums.begin(), nums.end());
        long long ans = LLONG_MAX;
        for (int i = 0; i <= 2; ++i) {
            int left = i;
            int right = n - 1 - (2 - i);
            if (left <= right) {
                ans = min(ans, (long long)nums[right] - nums[left]);
            }
        }
        return (int)ans;
    }
};