#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string reverseParentheses(string s) {
        int n = s.size();
        vector<int> pairIdx(n, -1);
        stack<int> st;
        for (int i = 0; i < n; ++i) {
            if (s[i] == '(') {
                st.push(i);
            } else if (s[i] == ')') {
                int j = st.top(); st.pop();
                pairIdx[i] = j;
                pairIdx[j] = i;
            }
        }
        string res;
        int i = 0, dir = 1;
        while (i < n) {
            if (s[i] == '(' || s[i] == ')') {
                i = pairIdx[i];
                dir = -dir;
            } else {
                res.push_back(s[i]);
            }
            i += dir;
        }
        return res;
    }
};