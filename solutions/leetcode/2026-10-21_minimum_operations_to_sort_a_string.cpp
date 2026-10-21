#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minOperations(string s) {
        int n = s.size();
        string t = s;
        sort(t.begin(), t.end());
        if (s == t) return 0;
        if (n == 2) return -1;
        int L = 0, R = n - 1;
        while (L < n && s[L] == t[L]) ++L;
        while (R >= 0 && s[R] == t[R]) --R;
        if (L == 0 && R == n - 1) return 2;
        return 1;
    }
};