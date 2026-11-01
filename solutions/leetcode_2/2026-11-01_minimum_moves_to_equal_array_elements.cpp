#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minMoves(vector<int>& nums) {
        long long sum = 0;
        int mn = nums[0];
        for (int x : nums) {
            sum += x;
            mn = min(mn, x);
        }
        return static_cast<int>(sum - 1LL * mn * nums.size());
    }
};