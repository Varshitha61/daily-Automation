#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int smallestIndex(vector<int>& nums) {
        for (int i = 0; i < (int)nums.size(); ++i) {
            int x = nums[i];
            int sum = 0;
            if (x == 0) sum = 0;
            while (x > 0) {
                sum += x % 10;
                x /= 10;
            }
            if (sum == i) return i;
        }
        return -1;
    }
};