#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int scoreOfParentheses(string s) {
        int ans = 0, bal = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (s[i] == '(') {
                ++bal;
            } else {
                --bal;
                if (i > 0 && s[i - 1] == '(') {
                    ans += 1 << bal;
                }
            }
        }
        return ans;
    }
};