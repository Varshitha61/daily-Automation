#include <bits/stdc++.h>
using namespace std;

class Solution {
    static const int MOD = 1000000007;
    vector<long long> fact, invFact;
    
    long long modpow(long long a, long long e) {
        long long r = 1;
        while (e) {
            if (e & 1) r = r * a % MOD;
            a = a * a % MOD;
            e >>= 1;
        }
        return r;
    }
    
    long long C(int n, int k) {
        if (k < 0 || k > n) return 0;
        return fact[n] * invFact[k] % MOD * invFact[n - k] % MOD;
    }
    
    pair<int,long long> dfs(const vector<int>& arr) {
        int n = arr.size();
        if (n <= 2) return {n, 1};
        int root = arr[0];
        vector<int> left, right;
        left.reserve(n);
        right.reserve(n);
        for (int i = 1; i < n; ++i) {
            if (arr[i] < root) left.push_back(arr[i]);
            else right.push_back(arr[i]);
        }
        auto L = dfs(left);
        auto R = dfs(right);
        long long ways = C(L.first + R.first, L.first);
        ways = ways * L.second % MOD;
        ways = ways * R.second % MOD;
        return {n, ways};
    }
    
public:
    int numberOfWays(vector<int>& nums) {
        int n = nums.size();
        fact.assign(n + 1, 0);
        invFact.assign(n + 1, 0);
        fact[0] = 1;
        for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i % MOD;
        invFact[n] = modpow(fact[n], MOD - 2);
        for (int i = n; i >= 1; --i) invFact[i - 1] = invFact[i] * i % MOD;
        auto res = dfs(nums);
        long long ans = (res.second - 1 + MOD) % MOD;
        return (int)ans;
    }
};