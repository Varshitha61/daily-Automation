#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maximumSum(vector<int>& nums, int k) {
        const int MOD = 1'000'000'007;
        const int MAXB = 31; // since nums[i] <= 1e9 < 2^30
        vector<int> cnt(MAXB, 0);
        for (int x : nums) {
            for (int b = 0; b < MAXB; ++b) {
                if (x & (1 << b)) cnt[b]++;
            }
        }
        long long ans = 0;
        for (int i = 0; i < k; ++i) {
            long long cur = 0;
            for (int b = 0; b < MAXB; ++b) {
                if (cnt[b] > i) cur += (1LL << b);
            }
            ans = (ans + (cur % MOD) * (cur % MOD)) % MOD;
        }
        return (int)ans;
    }
};