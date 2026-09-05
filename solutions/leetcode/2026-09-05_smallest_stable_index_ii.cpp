#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int smallestStableIndex(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> prefMax(n);
        vector<int> suffMin(n);
        prefMax[0] = nums[0];
        for (int i = 1; i < n; ++i) prefMax[i] = max(prefMax[i-1], nums[i]);
        suffMin[n-1] = nums[n-1];
        for (int i = n-2; i >= 0; --i) suffMin[i] = min(suffMin[i+1], nums[i]);
        for (int i = 0; i < n; ++i) {
            long long diff = (long long)prefMax[i] - (long long)suffMin[i];
            if (diff <= k) return i;
        }
        return -1;
    }
};