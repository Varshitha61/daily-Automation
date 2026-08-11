#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

int findSmallestMissingInteger(vector<int>& nums) {
    int n = nums.size();
    int maxLen = 0;
    int maxSum = 0;

    for (int i = 0; i < n; i++) {
        int len = 1;
        int sum = nums[i];
        for (int j = i + 1; j < n; j++) {
            if (nums[j] == nums[j - 1] + 1) {
                len++;
                sum += nums[j];
            } else {
                break;
            }
        }
        if (len > maxLen) {
            maxLen = len;
            maxSum = sum;
        }
    }

    unordered_set<int> numSet(nums.begin(), nums.end());
    int x = maxSum;
    while (numSet.find(x) != numSet.end()) {
        x++;
    }

    return x;
}

int main() {
    vector<int> nums1 = {1, 2, 3, 2, 5};
    vector<int> nums2 = {3, 4, 5, 1, 12, 14, 13};

    cout << findSmallestMissingInteger(nums1) << endl;  // Output: 6
    cout << findSmallestMissingInteger(nums2) << endl;  // Output: 15

    return 0;
}