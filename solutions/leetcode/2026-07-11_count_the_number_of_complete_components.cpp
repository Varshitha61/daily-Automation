#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class UnionFind {
public:
    vector<int> parent;
    vector<int> rank;

    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
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
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
};

int countCompleteComponents(int n, vector<vector<int>>& edges) {
    UnionFind uf(n);
    unordered_map<int, unordered_set<int>> graph;
    for (const auto& edge : edges) {
        int u = edge[0];
        int v = edge[1];
        uf.unionSet(u, v);
        graph[u].insert(v);
        graph[v].insert(u);
    }

    int count = 0;
    unordered_set<int> visited;
    for (int i = 0; i < n; i++) {
        if (visited.find(i) == visited.end()) {
            int root = uf.find(i);
            unordered_set<int> component;
            for (int j = 0; j < n; j++) {
                if (uf.find(j) == root) {
                    component.insert(j);
                    visited.insert(j);
                }
            }

            bool isComplete = true;
            for (int u : component) {
                for (int v : component) {
                    if (u != v && graph[u].find(v) == graph[u].end()) {
                        isComplete = false;
                        break;
                    }
                }
                if (!isComplete) {
                    break;
                }
            }

            if (isComplete) {
                count++;
            }
        }
    }

    return count;
}

int main() {
    int n = 6;
    vector<vector<int>> edges = {{0, 1}, {0, 2}, {1, 2}, {3, 4}};
    cout << countCompleteComponents(n, edges) << endl;

    n = 6;
    edges = {{0, 1}, {0, 2}, {1, 2}, {3, 4}, {3, 5}};
    cout << countCompleteComponents(n, edges) << endl;

    return 0;
}