#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minChanges(string s) {
        int n = s.size();
        int ans = 0;
        for (int i = 0; i < n; i += 2) {
            int cost0 = (s[i] != '0') + (s[i+1] != '0');
            int cost1 = (s[i] != '1') + (s[i+1] != '1');
            ans += min(cost0, cost1);
        }
        return ans;
    }
};