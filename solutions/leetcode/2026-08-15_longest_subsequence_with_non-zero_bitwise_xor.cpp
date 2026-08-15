class Solution {
public:
    int longestSubsequence(vector<int>& nums) {
        int n = nums.size();
        int ans = 0;
        for (int mask = 1; mask < (1 << n); mask++) {
            int xor_val = 0;
            for (int i = 0; i < n; i++) {
                if ((mask & (1 << i)) != 0) {
                    xor_val ^= nums[i];
                }
            }
            if (xor_val != 0) {
                ans = max(ans, __builtin_popcount(mask));
            }
        }
        return ans;
    }
};