#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minOperations(vector<int>& nums) {
        int n = nums.size();
        const int INF = 1e9;
        int ans = INF;
        // Check rotation case
        int rotK = ((0 - nums[0]) % n + n) % n;
        bool isRot = true;
        for (int i = 0; i < n; ++i) {
            if ( ((i - nums[i]) % n + n) % n != rotK ) {
                isRot = false;
                break;
            }
        }
        if (isRot) {
            int ops = min(rotK, n - rotK + 2);
            ans = min(ans, ops);
        }
        // Check reflection case
        int reflK = ((nums[0] - (n - 1 - 0)) % n + n) % n;
        bool isRefl = true;
        for (int i = 0; i < n; ++i) {
            if ( ((nums[i] - (n - 1 - i)) % n + n) % n != reflK ) {
                isRefl = false;
                break;
            }
        }
        if (isRefl) {
            int ops = 1 + min(reflK, n - reflK);
            ans = min(ans, ops);
        }
        return ans == INF ? -1 : ans;
    }
};