#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

int countTriplets(vector<int>& nums) {
    unordered_set<int> uniqueTriplets;
    for (int i = 0; i < nums.size(); i++) {
        for (int j = i; j < nums.size(); j++) {
            for (int k = j; k < nums.size(); k++) {
                int triplet = nums[i] ^ nums[j] ^ nums[k];
                uniqueTriplets.insert(triplet);
            }
        }
    }
    return uniqueTriplets.size();
}

int main() {
    vector<int> nums1 = {1, 3};
    vector<int> nums2 = {6, 7, 8, 9};
    cout << countTriplets(nums1) << endl;  // Output: 2
    cout << countTriplets(nums2) << endl;  // Output: 4
    return 0;
}