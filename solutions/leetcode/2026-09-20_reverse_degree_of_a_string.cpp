#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int reverseDegree(string s) {
        long long total = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            int revIdx = 'z' - s[i] + 1; // 'a'->26, 'z'->1
            total += 1LL * revIdx * (i + 1);
        }
        return (int)total;
    }
};