#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // Read input as per LeetCode style (not needed, but placeholder)
    // Since the platform expects a function, we implement a class solution.
    return 0;
}

class Solution {
public:
    vector<int> findSubtreeSizes(vector<int>& parent, string s) {
        int n = parent.size();
        vector<vector<int>> origAdj(n);
        for (int i = 1; i < n; ++i) {
            origAdj[parent[i]].push_back(i);
        }
        vector<int> newParent(n, -1);
        newParent[0] = -1;
        vector<vector<int>> charStack(26);
        // iterative DFS to avoid recursion depth issues
        struct Frame {int node; int state;}; // state 0 = pre, 1 = post
        vector<Frame> stack;
        stack.reserve(n*2);
        stack.push_back({0,0});
        while (!stack.empty()) {
            Frame cur = stack.back(); stack.pop_back();
            int u = cur.node;
            if (cur.state == 0) {
                // pre
                if (u != 0) {
                    int c = s[u]-'a';
                    if (!charStack[c].empty()) newParent[u] = charStack[c].back();
                    else newParent[u] = parent[u];
                }
                // push post state
                stack.push_back({u,1});
                // push children pre states
                for (int v : origAdj[u]) {
                    stack.push_back({v,0});
                }
                // push current node onto its char stack
                charStack[s[u]-'a'].push_back(u);
            } else {
                // post: pop from char stack
                charStack[s[u]-'a'].pop_back();
            }
        }
        // Build final adjacency
        vector<vector<int>> finalAdj(n);
        for (int i = 1; i < n; ++i) {
            finalAdj[newParent[i]].push_back(i);
        }
        // Compute subtree sizes
        vector<int> sz(n,0);
        vector<Frame> st2;
        st2.reserve(n*2);
        st2.push_back({0,0});
        while (!st2.empty()) {
            Frame cur = st2.back(); st2.pop_back();
            int u = cur.node;
            if (cur.state == 0) {
                st2.push_back({u,1});
                for (int v : finalAdj[u]) st2.push_back({v,0});
            } else {
                int sum = 1;
                for (int v : finalAdj[u]) sum += sz[v];
                sz[u] = sum;
            }
        }
        return sz;
    }
};