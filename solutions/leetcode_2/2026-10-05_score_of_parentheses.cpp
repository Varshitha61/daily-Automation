#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int scoreOfParentheses(string s) {
        int depth = 0, ans = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (s[i] == '(') {
                ++depth;
            } else {
                --depth;
                if (i > 0 && s[i-1] == '(') {
                    ans += 1 << depth;
                }
            }
        }
        return ans;
    }
};