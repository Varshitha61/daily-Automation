#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxSelectedElements(vector<int>& nums) {
        if (nums.empty()) return 0;
        int maxV = *max_element(nums.begin(), nums.end());
        int limit = maxV + 1; // maximum possible value after increment
        vector<int> freq(limit + 1, 0); // indices 0..limit
        for (int v : nums) {
            if (v <= limit) freq[v]++; // v is always <= maxV
        }
        long long cnt = freq[0]; // initially zero
        int L = 1;
        int ans = 0;
        for (int R = 1; R <= limit; ++R) {
            cnt += freq[R];
            while (L <= R && cnt < (long long)R - L + 1) {
                cnt -= freq[L - 1];
                ++L;
            }
            ans = max(ans, R - L + 1);
        }
        return ans;
    }
};