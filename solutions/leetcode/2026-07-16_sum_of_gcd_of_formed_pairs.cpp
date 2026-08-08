#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int sumOfGCDOfFormedPairs(vector<int>& nums) {
    int n = nums.size();
    vector<int> prefixGcd(n);
    int maxVal = nums[0];
    for (int i = 0; i < n; i++) {
        maxVal = max(maxVal, nums[i]);
        prefixGcd[i] = gcd(nums[i], maxVal);
    }
    sort(prefixGcd.begin(), prefixGcd.end());
    int sum = 0;
    for (int i = 0; i < n / 2; i++) {
        sum += gcd(prefixGcd[i], prefixGcd[n - i - 1]);
    }
    return sum;
}

int main() {
    vector<int> nums = {2, 6, 4};
    cout << sumOfGCDOfFormedPairs(nums) << endl;
    return 0;
}