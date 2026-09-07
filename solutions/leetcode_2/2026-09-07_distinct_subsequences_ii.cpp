#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int distinctSubseqII(string s) {
        const long long MOD = 1000000007LL;
        vector<long long> last(26, 0);
        long long total = 1; // empty subsequence
        for (char ch : s) {
            int idx = ch - 'a';
            long long newTotal = (total * 2 % MOD - last[idx] + MOD) % MOD;
            last[idx] = total;
            total = newTotal;
        }
        return (int)((total - 1 + MOD) % MOD);
    }
};