#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestMonotonicSubarray(vector<int>& nums) {
        if (nums.empty()) return 0;
        int maxLen = 1, incLen = 1, decLen = 1;
        for (size_t i = 1; i < nums.size(); ++i) {
            if (nums[i] > nums[i - 1]) {
                incLen++;
            } else {
                incLen = 1;
            }
            if (nums[i] < nums[i - 1]) {
                decLen++;
            } else {
                decLen = 1;
            }
            maxLen = max({maxLen, incLen, decLen});
        }
        return maxLen;
    }
};