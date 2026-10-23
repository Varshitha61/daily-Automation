#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maximumK(vector<int>& nums) {
        int n = nums.size();
        bool alreadySorted = true;
        vector<int> bad;
        for (int i = 0; i < n; ++i) {
            if (nums[i] != i) {
                alreadySorted = false;
                bad.push_back(i);
            }
        }
        if (alreadySorted) return 0;
        int k = 0;
        for (int b = 30; b >= 0; --b) {
            int cand = k | (1 << b);
            bool ok = true;
            for (int idx : bad) {
                if ( (idx & cand) != cand ) {
                    ok = false;
                    break;
                }
            }
            if (ok) k = cand;
        }
        return k;
    }
};