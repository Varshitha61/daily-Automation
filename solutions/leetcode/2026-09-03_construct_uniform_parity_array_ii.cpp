#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool constructArray(vector<int>& nums1) {
        int n = nums1.size();
        vector<pair<int,int>> arr;
        arr.reserve(n);
        for (int x : nums1) arr.emplace_back(x, x & 1);
        sort(arr.begin(), arr.end(), [](const auto& a, const auto& b){ return a.first < b.first; });
        bool seenParity[2] = {false, false};
        vector<char> canEven(n), canOdd(n);
        for (int i = 0; i < n; ++i) {
            int p = arr[i].second;
            bool evenPossible = (p == 0) || seenParity[p];
            bool oddPossible  = (p == 1) || seenParity[1 - p];
            canEven[i] = evenPossible;
            canOdd[i]  = oddPossible;
            seenParity[p] = true;
        }
        bool allEven = true, allOdd = true;
        for (int i = 0; i < n; ++i) {
            if (!canEven[i]) allEven = false;
            if (!canOdd[i])  allOdd = false;
        }
        return allEven || allOdd;
    }
};