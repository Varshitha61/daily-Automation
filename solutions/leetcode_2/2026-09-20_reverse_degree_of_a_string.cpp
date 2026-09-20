#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int reverseDegree(string s) {
        long long ans = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            ans += (long long)(i + 1) * ('z' - s[i] + 1);
        }
        return (int)ans;
    }
};