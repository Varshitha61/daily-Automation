#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool checkValidString(string s) {
        int low = 0, high = 0;
        for (char c : s) {
            if (c == '(') {
                low++;
                high++;
            } else if (c == ')') {
                low--;
                high--;
            } else { // '*'
                low--;      // treat as ')'
                high++;     // treat as '('
            }
            if (high < 0) return false;
            low = max(low, 0);
        }
        return low == 0;
    }
};