#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minMoves2(vector<int>& nums) {
        int n = nums.size();
        int mid = n / 2;
        nth_element(nums.begin(), nums.begin() + mid, nums.end());
        int median = nums[mid];
        long long moves = 0;
        for (int x : nums) moves += llabs((long long)x - median);
        return (int)moves;
    }
};