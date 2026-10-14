#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string maxNumber(int num, int sum) {
        if (sum > 9LL * num) return "";
        long long k = sum / 9;
        int r = sum % 9;
        string res;
        res.append(k, '9');
        if (r > 0) res.push_back('0' + r);
        if ((int)res.size() < num) res.append(num - res.size(), '0');
        return res;
    }
};