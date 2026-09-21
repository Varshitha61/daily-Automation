#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<long long> findXValue(vector<int>& nums, int k) {
        vector<long long> ans(k, 0);
        vector<long long> prev(k, 0), cur(k, 0);
        for (int v : nums) {
            int a = v % k;
            fill(cur.begin(), cur.end(), 0);
            for (int mod = 0; mod < k; ++mod) {
                if (prev[mod]) {
                    int newMod = (int)((1LL * mod * a) % k);
                    cur[newMod] += prev[mod];
                }
            }
            cur[a] += 1; // subarray of length 1
            for (int mod = 0; mod < k; ++mod) {
                ans[mod] += cur[mod];
            }
            prev.swap(cur);
        }
        return ans;
    }
};