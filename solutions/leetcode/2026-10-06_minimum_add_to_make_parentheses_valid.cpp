#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minAddToMakeValid(string s) {
        int need = 0, bal = 0;
        for (char c : s) {
            if (c == '(') {
                ++bal;
            } else {
                if (bal > 0) --bal;
                else ++need;
            }
        }
        return need + bal;
    }
};