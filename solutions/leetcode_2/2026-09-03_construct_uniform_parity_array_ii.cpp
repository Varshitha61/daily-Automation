#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool canMakeUniformParity(vector<int>& nums1) {
        if (nums1.empty()) return true;
        int minVal = *min_element(nums1.begin(), nums1.end());
        if (minVal % 2 == 1) return true; // min is odd, target odd, always possible
        // min is even, need all numbers even
        for (int x : nums1) {
            if (x % 2 == 1) return false;
        }
        return true;
    }
};