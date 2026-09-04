#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int smallestStableIndex(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> prefMax(n), suffMin(n);
        for (int i = 0; i < n; ++i) {
            prefMax[i] = (i == 0) ? nums[i] : max(prefMax[i - 1], nums[i]);
        }
        for (int i = n - 1; i >= 0; --i) {
            suffMin[i] = (i == n - 1) ? nums[i] : min(suffMin[i + 1], nums[i]);
        }
        for (int i = 0; i < n; ++i) {
            long long diff = (long long)prefMax[i] - (long long)suffMin[i];
            if (diff <= k) return i;
        }
        return -1;
    }
};