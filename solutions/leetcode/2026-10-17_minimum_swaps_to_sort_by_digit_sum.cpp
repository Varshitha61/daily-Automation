#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minimumSwaps(vector<int>& nums) {
        int n = nums.size();
        vector<tuple<int,int,int>> arr; // digit sum, value, original index
        arr.reserve(n);
        for (int i = 0; i < n; ++i) {
            int x = nums[i];
            int sum = 0;
            while (x) { sum += x % 10; x /= 10; }
            arr.emplace_back(sum, nums[i], i);
        }
        sort(arr.begin(), arr.end(), [](const auto& a, const auto& b){
            if (get<0>(a) != get<0>(b)) return get<0>(a) < get<0>(b);
            return get<1>(a) < get<1>(b);
        });
        vector<int> targetPos(n);
        for (int i = 0; i < n; ++i) {
            int origIdx = get<2>(arr[i]);
            targetPos[origIdx] = i;
        }
        vector<char> visited(n, 0);
        long long swaps = 0;
        for (int i = 0; i < n; ++i) {
            if (visited[i] || targetPos[i] == i) continue;
            int cycleSize = 0;
            int j = i;
            while (!visited[j]) {
                visited[j] = 1;
                j = targetPos[j];
                ++cycleSize;
            }
            if (cycleSize > 0) swaps += cycleSize - 1;
        }
        return (int)swaps;
    }
};