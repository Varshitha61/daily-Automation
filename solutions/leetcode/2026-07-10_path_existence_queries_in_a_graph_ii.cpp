#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>

using namespace std;

class Solution {
public:
    vector<int> distance(vector<vector<int>>& queries, int n, vector<int>& nums, int maxDiff) {
        vector<int> result;
        unordered_map<int, vector<int>> graph;
        
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (abs(nums[i] - nums[j]) <= maxDiff) {
                    graph[i].push_back(j);
                    graph[j].push_back(i);
                }
            }
        }
        
        for (auto& query : queries) {
            int u = query[0], v = query[1];
            if (u == v) {
                result.push_back(0);
                continue;
            }
            
            unordered_map<int, int> distance;
            queue<int> q;
            q.push(u);
            distance[u] = 0;
            
            while (!q.empty()) {
                int node = q.front();
                q.pop();
                
                for (int neighbor : graph[node]) {
                    if (distance.find(neighbor) == distance.end()) {
                        distance[neighbor] = distance[node] + 1;
                        q.push(neighbor);
                    }
                }
            }
            
            if (distance.find(v) != distance.end()) {
                result.push_back(distance[v]);
            } else {
                result.push_back(-1);
            }
        }
        
        return result;
    }
};

int main() {
    Solution solution;
    int n, maxDiff;
    vector<int> nums;
    vector<vector<int>> queries;
    
    // Test case 1
    n = 5;
    nums = {1, 8, 3, 4, 2};
    maxDiff = 3;
    queries = {{0, 3}, {2, 4}};
    vector<int> result1 = solution.distance(queries, n, nums, maxDiff);
    for (int i : result1) {
        cout << i << " ";
    }
    cout << endl;
    
    // Test case 2
    n = 5;
    nums = {5, 3, 1, 9, 10};
    maxDiff = 2;
    queries = {{0, 1}, {0, 2}, {2, 3}, {4, 3}};
    vector<int> result2 = solution.distance(queries, n, nums, maxDiff);
    for (int i : result2) {
        cout << i << " ";
    }
    cout << endl;
    
    // Test case 3
    n = 3;
    nums = {3, 6, 1};
    maxDiff = 1;
    queries = {{0, 0}, {0, 1}, {1, 2}};
    vector<int> result3 = solution.distance(queries, n, nums, maxDiff);
    for (int i : result3) {
        cout << i << " ";
    }
    cout << endl;
    
    return 0;
}