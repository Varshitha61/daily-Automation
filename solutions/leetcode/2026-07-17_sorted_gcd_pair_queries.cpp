#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

vector<int> gcdPairs(vector<int>& nums, vector<int>& queries) {
    vector<int> gcds;
    for (int i = 0; i < nums.size(); i++) {
        for (int j = i + 1; j < nums.size(); j++) {
            gcds.push_back(gcd(nums[i], nums[j]));
        }
    }
    sort(gcds.begin(), gcds.end());
    vector<int> result;
    for (int query : queries) {
        result.push_back(gcds[query]);
    }
    return result;
}

int main() {
    vector<int> nums = {2, 3, 4};
    vector<int> queries = {0, 2, 2};
    vector<int> result = gcdPairs(nums, queries);
    for (int i : result) {
        cout << i << " ";
    }
    return 0;
}