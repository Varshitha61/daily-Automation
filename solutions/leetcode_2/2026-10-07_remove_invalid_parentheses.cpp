#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<string> removeInvalidParentheses(string s) {
        int leftRem = 0, rightRem = 0;
        for (char c : s) {
            if (c == '(') {
                leftRem++;
            } else if (c == ')') {
                if (leftRem > 0) leftRem--;
                else rightRem++;
            }
        }
        unordered_set<string> resSet;
        string path;
        function<void(int,int,int,int,int)> dfs = [&](int idx, int leftCount, int rightCount, int lRem, int rRem) {
            if (idx == (int)s.size()) {
                if (lRem == 0 && rRem == 0 && leftCount == rightCount) {
                    resSet.insert(path);
                }
                return;
            }
            char c = s[idx];
            if (c == '(') {
                if (lRem > 0) {
                    dfs(idx + 1, leftCount, rightCount, lRem - 1, rRem); // skip
                }
                path.push_back(c);
                dfs(idx + 1, leftCount + 1, rightCount, lRem, rRem);
                path.pop_back();
            } else if (c == ')') {
                if (rRem > 0) {
                    dfs(idx + 1, leftCount, rightCount, lRem, rRem - 1); // skip
                }
                if (leftCount > rightCount) {
                    path.push_back(c);
                    dfs(idx + 1, leftCount, rightCount + 1, lRem, rRem);
                    path.pop_back();
                }
            } else {
                path.push_back(c);
                dfs(idx + 1, leftCount, rightCount, lRem, rRem);
                path.pop_back();
            }
        };
        dfs(0, 0, 0, leftRem, rightRem);
        return vector<string>(resSet.begin(), resSet.end());
    }
};