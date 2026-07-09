#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

class UnionFind {
public:
    vector<int> parent;
    vector<int> rank;

    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unionSet(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
};

class Solution {
public:
    vector<bool> areConnected(int n, vector<int>& nums, int maxDiff, vector<vector<int>>& queries) {
        UnionFind uf(n);
        for (int i = 0; i < n - 1; i++) {
            if (nums[i + 1] - nums[i] <= maxDiff) {
                uf.unionSet(i, i + 1);
            }
        }
        vector<bool> result;
        for (auto& query : queries) {
            result.push_back(uf.find(query[0]) == uf.find(query[1]));
        }
        return result;
    }
};

int main() {
    Solution solution;
    int n = 4;
    vector<int> nums = {2, 5, 6, 8};
    int maxDiff = 2;
    vector<vector<int>> queries = {{0, 1}, {0, 2}, {1, 3}, {2, 3}};
    vector<bool> result = solution.areConnected(n, nums, maxDiff, queries);
    for (bool b : result) {
        cout << b << " ";
    }
    return 0;
}