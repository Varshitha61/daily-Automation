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
        int p = (n + 1) / 2;
        nth_element(sq.begin(), sq.begin() + p, sq.end(), greater<long long>());
        long long topSum = 0;
        for (int i = 0; i < p; ++i) topSum += sq[i];
        return 2 * topSum - total;
    }
};