#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int smallestIndex(vector<int>& nums) {
        auto digitSum = [](int x) {
            int s = 0;
            if (x == 0) return 0;
            while (x > 0) {
                s += x % 10;
                x /= 10;
            }
            return s;
        };
        for (int i = 0; i < (int)nums.size(); ++i) {
            if (digitSum(nums[i]) == i) return i;
        }
        return -1;
    }
};