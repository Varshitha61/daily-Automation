#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxPalindromes(string s, int k) {
        int n = s.size();
        vector<vector<int>> endsAt(n);
        // odd length palindromes
        for (int center = 0; center < n; ++center) {
            int l = center, r = center;
            while (l >= 0 && r < n && s[l] == s[r]) {
                if (r - l + 1 >= k) endsAt[r].push_back(l);
                --l; ++r;
            }
        }
        // even length palindromes
        for (int center = 0; center < n - 1; ++center) {
            int l = center, r = center + 1;
            while (l >= 0 && r < n && s[l] == s[r]) {
                if (r - l + 1 >= k) endsAt[r].push_back(l);
                --l; ++r;
            }
        }
        const int INF_NEG = -1e9;
        vector<int> dp(n + 1, INF_NEG);
        dp[0] = 0;
        for (int i = 0; i < n; ++i) {
            // skip character i
            dp[i + 1] = max(dp[i + 1], dp[i]);
            // use any palindrome ending at i
            for (int l : endsAt[i]) {
                dp[i + 1] = max(dp[i + 1], dp[l] + 1);
            }
        }
        return dp[n];
    }
};