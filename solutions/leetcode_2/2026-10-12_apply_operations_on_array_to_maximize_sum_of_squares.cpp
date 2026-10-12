#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxSum(vector<int>& nums, int k) {
        const long long MOD = 1000000007LL;
        const int MAXB = 31; // up to 2^30
        vector<long long> cnt(MAXB, 0);
        for (int x : nums) {
            for (int b = 0; b < MAXB; ++b) {
                if (x & (1 << b)) cnt[b]++;
            }
        }
        priority_queue<long long> pq;
        for (int i = 0; i < k; ++i) pq.push(0LL);
        for (int b = MAXB - 1; b >= 0; --b) {
            long long c = cnt[b];
            int t = (int)min<long long>(c, k);
            vector<long long> temp;
            temp.reserve(t);
            for (int i = 0; i < t; ++i) {
                long long val = pq.top(); pq.pop();
                val += (1LL << b);
                temp.push_back(val);
            }
            for (long long v : temp) pq.push(v);
        }
        long long ans = 0;
        while (!pq.empty()) {
            long long v = pq.top(); pq.pop();
            long long vm = v % MOD;
            __int128 prod = (__int128)vm * vm;
            ans = (ans + (long long)(prod % MOD)) % MOD;
        }
        return (int)ans;
    }
};