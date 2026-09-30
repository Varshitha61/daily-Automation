#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> maxDepthAfterSplit(string seq) {
        vector<int> ans;
        ans.reserve(seq.size());
        int depth = 0;
        for (char c : seq) {
            if (c == '(') {
                depth++;
                ans.push_back(depth % 2);
            } else { // ')'
                ans.push_back(depth % 2);
                depth--;
            }
        }
        return ans;
    }
};