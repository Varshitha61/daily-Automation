#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maximumK(vector<int>& nums) {
        int n = nums.size();
        int ans = -1; // all bits set
        bool any = false;
        for (int i = 0; i < n; ++i) {
            if (nums[i] != i) {
                int cur = i & nums[i];
                if (!any) {
                    ans = cur;
                    any = true;
                } else {
                    ans &= cur;
                }
            }
        }
        return any ? ans : 0;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<int> nums;
    int x;
    while (cin >> x) {
        nums.push_back(x);
    }
    if (nums.empty()) return 0;
    Solution sol;
    cout << sol.maximumK(nums);
    return 0;
}