#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool checkString(string s) {
        bool seenB = false;
        for (char c : s) {
            if (c == 'b') {
                seenB = true;
            } else { // c == 'a'
                if (seenB) return false;
            }
        }
        return true;
    }
};