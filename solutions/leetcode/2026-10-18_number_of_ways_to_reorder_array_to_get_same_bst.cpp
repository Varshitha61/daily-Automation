#include <bits/stdc++.h>
using namespace std;

class Solution {
    static const int MOD = 1'000'000'007;
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
    
    struct Node {
        int val;
        Node *left = nullptr, *right = nullptr;
        Node(int v): val(v) {}
    };
    
    void insert(Node* &root, int v) {
        if (!root) { root = new Node(v); return; }
        if (v < root->val) insert(root->left, v);
        else insert(root->right, v);
    }
    
    pair<int,long long> dfs(Node* node) {
        if (!node) return {0,1};
        auto left = dfs(node->left);
        auto right = dfs(node->right);
        int lsz = left.first, rsz = right.first;
        long long ways = C(lsz + rsz, lsz);
        ways = ways * left.second % MOD;
        ways = ways * right.second % MOD;
        return {lsz + rsz + 1, ways};
    }
    
public:
    int numOfWays(vector<int>& nums) {
        int n = nums.size();
        fact.resize(n+1);
        invFact.resize(n+1);
        fact[0] = 1;
        for (int i=1;i<=n;i++) fact[i] = fact[i-1]*i%MOD;
        invFact[n] = modpow(fact[n], MOD-2);
        for (int i=n;i>0;i--) invFact[i-1] = invFact[i]*i%MOD;
        
        Node* root = nullptr;
        for (int v: nums) insert(root, v);
        long long totalWays = dfs(root).second;
        long long ans = (totalWays - 1 + MOD) % MOD;
        return (int)ans;
    }
};