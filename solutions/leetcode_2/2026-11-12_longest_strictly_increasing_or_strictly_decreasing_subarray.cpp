#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestMonotonicSubarray(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        int inc = 1, dec = 1, ans = 1;
        for (int i = 1; i < n; ++i) {
            if (nums[i] > nums[i - 1]) inc++;
            else inc = 1;
            if (nums[i] < nums[i - 1]) dec++;
            else dec = 1;
            ans = max({ans, inc, dec});
        }
        return ans;
    }
};