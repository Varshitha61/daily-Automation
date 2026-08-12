#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int longestSubarray(vector<int>& nums, int k) {
    int n = nums.size();
    int maxLen = 0;
    unordered_map<int, int> freq;

    int left = 0;
    for (int right = 0; right < n; right++) {
        freq[nums[right]]++;

        while (freq[nums[right]] > k) {
            freq[nums[left]]--;
            if (freq[nums[left]] == 0) {
                freq.erase(nums[left]);
            }
            left++;
        }

        maxLen = max(maxLen, right - left + 1);
    }

    return maxLen;
}

int main() {
    vector<int> nums = {1, 2, 3, 1, 2, 3, 1, 2};
    int k = 2;
    cout << longestSubarray(nums, k) << endl;

    nums = {1, 2, 1, 2, 1, 2, 1, 2};
    k = 1;
    cout << longestSubarray(nums, k) << endl;

    nums = {5, 5, 5, 5, 5, 5, 5};
    k = 4;
    cout << longestSubarray(nums, k) << endl;

    return 0;
}