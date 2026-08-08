#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class Solution {
public:
    vector<int> findUninvokedMethods(int n, int k, vector<vector<int>>& invocations) {
        vector<int> result;
        vector<unordered_set<int>> graph(n);
        vector<int> in_degree(n, 0);
        
        for (const auto& invocation : invocations) {
            int caller = invocation[0];
            int callee = invocation[1];
            graph[caller].insert(callee);
            in_degree[callee]++;
        }
        
        unordered_set<int> suspicious;
        dfs(k, suspicious, graph);
        
        bool can_remove = true;
        for (const auto& invocation : invocations) {
            int caller = invocation[0];
            int callee = invocation[1];
            if (suspicious.find(callee) != suspicious.end() && suspicious.find(caller) == suspicious.end()) {
                can_remove = false;
                break;
            }
        }
        
        if (can_remove) {
            for (int i = 0; i < n; i++) {
                if (suspicious.find(i) == suspicious.end()) {
                    result.push_back(i);
                }
            }
        } else {
            for (int i = 0; i < n; i++) {
                result.push_back(i);
            }
        }
        
        return result;
    }
    
private:
    void dfs(int node, unordered_set<int>& suspicious, const vector<unordered_set<int>>& graph) {
        if (suspicious.find(node) != suspicious.end()) {
            return;
        }
        suspicious.insert(node);
        for (const auto& neighbor : graph[node]) {
            dfs(neighbor, suspicious, graph);
        }
    }
};

int main() {
    Solution solution;
    int n = 4;
    int k = 1;
    vector<vector<int>> invocations = {{1,2},{0,1},{3,2}};
    vector<int> result = solution.findUninvokedMethods(n, k, invocations);
    for (const auto& method : result) {
        cout << method << " ";
    }
    cout << endl;
    return 0;
}