#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string maxScore(int num, int sum) {
        long long n = num;
        long long s = sum;
        if (s > 9LL * n) return "";
        long long cnt9 = s / 9;
        int rem = s % 9;
        if (cnt9 > n) return "";
        string res;
        res.append(cnt9, '9');
        if (rem > 0) {
            res.push_back(char('0' + rem));
        }
        long long zeros = n - cnt9 - (rem > 0 ? 1 : 0);
        res.append(zeros, '0');
        // Ensure leading digit is not zero (should hold by construction)
        if (!res.empty() && res[0] == '0') return "";
        return res;
    }
};