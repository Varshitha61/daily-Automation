#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long minScore(vector<int>& nums) {
        int n = nums.size();
        sort(nums.begin(), nums.end());
        long long ans = LLONG_MAX;
        // remove 0 left, 2 right
        ans = min(ans, (long long)nums[n-3] - nums[0]);
        // remove 1 left, 1 right
        ans = min(ans, (long long)nums[n-2] - nums[1]);
        // remove 2 left, 0 right
        ans = min(ans, (long long)nums[n-1] - nums[2]);
        return ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<int> nums;
    int x;
    // Read input as per examples: assume first line is array like [1,4,7,8,5]
    string line;
    if (!getline(cin, line)) return 0;
    // extract numbers
    string num;
    for (char c : line) {
        if (isdigit(c) || c=='-') num += c;
        else {
            if (!num.empty()) {
                nums.push_back(stoi(num));
                num.clear();
            }
        }
    }
    if (!num.empty()) nums.push_back(stoi(num));
    Solution sol;
    cout << sol.minScore(nums);
    return 0;
}