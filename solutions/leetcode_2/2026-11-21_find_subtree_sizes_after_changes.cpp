#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> findSubtreeSizes(vector<int>& parent, string s) {
        int n = parent.size();
        vector<vector<int>> children(n);
        for (int i = 1; i < n; ++i) {
            children[parent[i]].push_back(i);
        }
        vector<int> nearestSame(n, -1);
        vector<vector<int>> charStack(26);
        // iterative DFS with entry/exit states
        vector<pair<int,int>> stk;
        stk.reserve(2*n);
        stk.emplace_back(0, 0); // node, state 0=enter,1=exit
        while (!stk.empty()) {
            auto [u, state] = stk.back();
            stk.pop_back();
            int c = s[u] - 'a';
            if (state == 0) {
                // entering
                if (!charStack[c].empty()) nearestSame[u] = charStack[c].back();
                else nearestSame[u] = -1;
                charStack[c].push_back(u);
                // push exit state
                stk.emplace_back(u, 1);
                // push children enter states
                for (int v : children[u]) {
                    stk.emplace_back(v, 0);
                }
            } else {
                // exiting
                charStack[c].pop_back();
            }
        }
        // build new parent array
        vector<int> newParent = parent;
        for (int i = 1; i < n; ++i) {
            if (nearestSame[i] != -1) newParent[i] = nearestSame[i];
        }
        // build new children adjacency
        vector<vector<int>> newChildren(n);
        for (int i = 1; i < n; ++i) {
            newChildren[newParent[i]].push_back(i);
        }
        // compute subtree sizes via post-order
        vector<int> order;
        order.reserve(n);
        vector<int> stack;
        stack.push_back(0);
        while (!stack.empty()) {
            int u = stack.back(); stack.pop_back();
            order.push_back(u);
            for (int v : newChildren[u]) stack.push_back(v);
        }
        vector<int> sz(n, 1);
        for (int i = n-1; i >= 0; --i) {
            int u = order[i];
            for (int v : newChildren[u]) sz[u] += sz[v];
        }
        return sz;
    }
};