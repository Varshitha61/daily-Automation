#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long countSubarrays(vector<int>& nums, long long k) {
        int n = nums.size();
        long long ans = 0;
        int left = 0, right = 0;
        long long windowSum = 0;
        long long totalMaxSum = 0;
        deque<pair<long long,long long>> seg; // {value, count}
        while (left < n) {
            while (right < n) {
                long long x = nums[right];
                long long add = seg.empty() || x > seg.back().first ? x : seg.back().first;
                if (totalMaxSum + add - (windowSum + x) <= k) {
                    windowSum += x;
                    totalMaxSum += add;
                    if (seg.empty() || x > seg.back().first) {
                        seg.emplace_back(x, 1);
                    } else {
                        seg.back().second += 1;
                    }
                    ++right;
                } else break;
            }
            ans += (right - left);
            if (left == right) {
                ++left;
                right = left;
                seg.clear();
                windowSum = 0;
                totalMaxSum = 0;
            } else {
                long long leftVal = nums[left];
                windowSum -= leftVal;
                totalMaxSum -= seg.front().first;
                seg.front().second -= 1;
                if (seg.front().second == 0) seg.pop_front();
                ++left;
            }
        }
        return ans;
    }
};