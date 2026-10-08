#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string removeOuterParentheses(string s) {
        string res;
        int depth = 0;
        for (char c : s) {
            if (c == '(') {
                if (depth > 0) res.push_back(c);
                ++depth;
            } else { // c == ')'
                --depth;
                if (depth > 0) res.push_back(c);
            }
        }
        return res;
    }
};