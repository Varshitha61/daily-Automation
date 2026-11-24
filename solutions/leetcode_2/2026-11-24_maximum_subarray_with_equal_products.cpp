#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestSubarray(vector<int>& nums) {
        int n = nums.size();
        vector<int> masks(n);
        for (int i = 0; i < n; ++i) {
            int x = nums[i];
            int m = 0;
            if (x % 2 == 0) m |= 1<<0;
            if (x % 3 == 0) m |= 1<<1;
            if (x % 5 == 0) m |= 1<<2;
            if (x % 7 == 0) m |= 1<<3;
            masks[i] = m;
        }
        int left = 0, used = 0, ans = 0;
        for (int right = 0; right < n; ++right) {
            int cur = masks[right];
            while ((used & cur) != 0) {
                used ^= masks[left];
                ++left;
            }
            used |= cur;
            ans = max(ans, right - left + 1);
        }
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<int> nums;
    int val;
    // Read input as a list like [1,2,3]
    string s;
    if (!getline(cin, s)) return 0;
    // remove brackets and spaces
    for (char &c : s) if (c == '[' || c == ']' || c == ',') c = ' ';
    stringstream ss(s);
    while (ss >> val) nums.push_back(val);
    Solution sol;
    cout << sol.longestSubarray(nums);
    return 0;
}