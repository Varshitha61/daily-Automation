#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int sumOfSquares(vector<int>& nums) {
        int n = nums.size();
        long long sum = 0;
        for (int i = 1; i <= n; ++i) {
            if (n % i == 0) {
                long long val = nums[i - 1];
                sum += val * val;
            }
        }
        return (int)sum;
    }
};