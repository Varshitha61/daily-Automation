#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

int count_unique_triplets(vector<int>& nums) {
    unordered_set<int> unique_triplets;
    int n = nums.size();

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            for (int k = j; k < n; k++) {
                int triplet = nums[i] ^ nums[j] ^ nums[k];
                unique_triplets.insert(triplet);
            }
        }
    }

    return unique_triplets.size();
}

int main() {
    vector<int> nums1 = {1, 2};
    vector<int> nums2 = {3, 1, 2};

    cout << count_unique_triplets(nums1) << endl;  // Output: 2
    cout << count_unique_triplets(nums2) << endl;  // Output: 4

    return 0;
}