#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long maxAlternatingScore(vector<int>& nums) {
        int n = nums.size();
        vector<long long> sq(n);
        long long total = 0;
        for (int i = 0; i < n; ++i) {
            long long v = 1LL * nums[i] * nums[i];
            sq[i] = v;
            total += v;
        }
        sort(sq.begin(), sq.end());
        int m = n / 2; // number of negatives
        long long negSum = 0;
        for (int i = 0; i < m; ++i) negSum += sq[i];
        return total - 2 * negSum;
    }
};