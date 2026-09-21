#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<long long> findXValues(vector<int>& nums, int k) {
        vector<long long> ans(k, 0);
        vector<long long> prev(k, 0), cur(k, 0);
        for (int val : nums) {
            fill(cur.begin(), cur.end(), 0);
            int modVal = val % k;
            cur[modVal] += 1; // subarray consisting of only this element
            for (int r = 0; r < k; ++r) {
                if (prev[r] == 0) continue;
                int newR = (int)((1LL * r * modVal) % k);
                cur[newR] += prev[r];
            }
            for (int r = 0; r < k; ++r) ans[r] += cur[r];
            prev.swap(cur);
        }
        return ans;
    }
};