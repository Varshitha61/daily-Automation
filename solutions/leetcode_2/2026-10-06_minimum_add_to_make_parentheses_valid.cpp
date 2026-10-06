#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minAddToMakeValid(string s) {
        int balance = 0, insertions = 0;
        for (char c : s) {
            if (c == '(') {
                ++balance;
            } else {
                if (balance > 0) {
                    --balance;
                } else {
                    ++insertions;
                }
            }
        }
        return insertions + balance;
    }
};