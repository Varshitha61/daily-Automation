#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minMoves(vector<int>& nums) {
        int mx = *max_element(nums.begin(), nums.end());
        long long moves = 0;
        for (int v : nums) moves += mx - v;
        return (int)moves;
    }
};