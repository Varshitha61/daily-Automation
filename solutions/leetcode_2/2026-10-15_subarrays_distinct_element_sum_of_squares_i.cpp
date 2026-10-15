#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long subarraysDistinctElementSumOfSquaresI(vector<int>& nums) {
        int n = nums.size();
        long long ans = 0;
        const int MAXV = 101;
        for (int i = 0; i < n; ++i) {
            int freq[MAXV] = {0};
            int distinct = 0;
            for (int j = i; j < n; ++j) {
                if (freq[nums[j]] == 0) ++distinct;
                ++freq[nums[j]];
                ans += 1LL * distinct * distinct;
            }
        }
        return ans;
    }
};