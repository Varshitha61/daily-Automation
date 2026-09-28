#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxDepth(string s) {
        int cur = 0, ans = 0;
        for (char c : s) {
            if (c == '(') {
                ++cur;
                ans = max(ans, cur);
            } else if (c == ')') {
                --cur;
            }
        }
        return ans;
    }
};