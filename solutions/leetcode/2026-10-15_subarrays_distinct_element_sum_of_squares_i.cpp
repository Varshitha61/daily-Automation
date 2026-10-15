#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long subarraysDistinctElementSumOfSquaresI(vector<int>& nums) {
        int n = nums.size();
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            vector<int> seen(101, 0);
            int distinct = 0;
            for (int j = i; j < n; ++j) {
                if (!seen[nums[j]]) {
                    seen[nums[j]] = 1;
                    ++distinct;
                }
                ans += 1LL * distinct * distinct;
            }
        }
        return ans;
    }
};