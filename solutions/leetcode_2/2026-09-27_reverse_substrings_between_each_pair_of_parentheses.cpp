#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string reverseParentheses(string s) {
        vector<string> st(1);
        for (char c : s) {
            if (c == '(') {
                st.emplace_back();
            } else if (c == ')') {
                string cur = st.back();
                st.pop_back();
                reverse(cur.begin(), cur.end());
                st.back() += cur;
            } else {
                st.back().push_back(c);
            }
        }
        return st[0];
    }
};