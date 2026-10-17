#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    static int digitSum(int x) {
        int s = 0;
        while (x) {
            s += x % 10;
            x /= 10;
        }
        return s;
    }
    
    int minSwaps(vector<int>& nums) {
        int n = nums.size();
        vector<int> sorted = nums;
        sort(sorted.begin(), sorted.end(), [&](int a, int b) {
            int sa = digitSum(a), sb = digitSum(b);
            if (sa != sb) return sa < sb;
            return a < b;
        });
        unordered_map<int,int> targetPos;
        targetPos.reserve(n * 2);
        for (int i = 0; i < n; ++i) targetPos[sorted[i]] = i;
        vector<int> to(n);
        for (int i = 0; i < n; ++i) to[i] = targetPos[nums[i]];
        vector<char> visited(n, 0);
        int cycles = 0;
        for (int i = 0; i < n; ++i) {
            if (visited[i]) continue;
            ++cycles;
            int j = i;
            while (!visited[j]) {
                visited[j] = 1;
                j = to[j];
            }
        }
        return n - cycles;
    }
};