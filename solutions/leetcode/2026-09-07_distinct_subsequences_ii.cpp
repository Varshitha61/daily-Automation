#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int distinctSubseqII(string s) {
        const long long MOD = 1000000007LL;
        int n = s.size();
        vector<long long> dp(n + 1, 0);
        dp[0] = 1; // empty subsequence
        vector<int> last(26, -1);
        for (int i = 1; i <= n; ++i) {
            int idx = s[i - 1] - 'a';
            dp[i] = (dp[i - 1] * 2) % MOD;
            if (last[idx] != -1) {
                dp[i] = (dp[i] - dp[last[idx] - 1] + MOD) % MOD;
            }
            last[idx] = i;
        }
        long long ans = (dp[n] - 1 + MOD) % MOD; // exclude empty subsequence
        return (int)ans;
    }
};