#include <iostream>
#include <vector>
#include <algorithm>

class Solution {
public:
    std::vector<int> findMissingNumbers(std::vector<int>& nums) {
        int min_val = *std::min_element(nums.begin(), nums.end());
        int max_val = *std::max_element(nums.begin(), nums.end());
        std::vector<int> full_range;
        for (int i = min_val; i <= max_val; i++) {
            full_range.push_back(i);
        }
        std::vector<int> missing_nums;
        for (int num : full_range) {
            if (std::find(nums.begin(), nums.end(), num) == nums.end()) {
                missing_nums.push_back(num);
            }
        }
        return missing_nums;
    }
};

int main() {
    Solution solution;
    std::vector<int> nums1 = {1, 4, 2, 5};
    std::vector<int> result1 = solution.findMissingNumbers(nums1);
    for (int num : result1) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::vector<int> nums2 = {7, 8, 6, 9};
    std::vector<int> result2 = solution.findMissingNumbers(nums2);
    for (int num : result2) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::vector<int> nums3 = {5, 1};
    std::vector<int> result3 = solution.findMissingNumbers(nums3);
    for (int num : result3) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}