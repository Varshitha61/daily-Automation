#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minInsertions(string s) {
        int ans = 0;
        int need = 0; // number of ')' needed
        for (char c : s) {
            if (c == '(') {
                need += 2;
                if (need % 2) { // odd, insert one ')'
                    ans++;
                    need--;
                }
            } else { // ')'
                need--;
                if (need < 0) {
                    ans++;      // insert '('
                    need = 1;   // after inserting '(', we need one more ')'
                }
            }
        }
        return ans + need;
    }
};