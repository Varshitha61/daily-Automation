#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>

using namespace std;

class Solution {
public:
    vector<int> shortestDistance(vector<int>& nums, int maxDiff, vector<vector<int>>& queries) {
        int n = nums.size();
        vector<int> result;
        for (auto& query : queries) {
            int u = query[0], v = query[1];
            if (u == v) {
                result.push_back(0);
                continue;
            }
            unordered_map<int, vector<int>> graph;
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    if (abs(nums[i] - nums[j]) <= maxDiff) {
                        graph[i].push_back(j);
                        graph[j].push_back(i);
                    }
                }
            }
            queue<pair<int, int>> q;
            q.push({u, 0});
            unordered_set<int> visited;
            bool found = false;
            while (!q.empty()) {
                auto [node, dist] = q.front();
                q.pop();
                if (node == v) {
                    result.push_back(dist);
                    found = true;
                    break;
                }
                if (visited.find(node) != visited.end()) continue;
                visited.insert(node);
                for (auto& neighbor : graph[node]) {
                    q.push({neighbor, dist + 1});
                }
            }
            if (!found) result.push_back(-1);
        }
        return result;
    }
};

int main() {
    Solution solution;
    vector<int> nums1 = {1, 8, 3, 4, 2};
    int maxDiff1 = 3;
    vector<vector<int>> queries1 = {{0, 3}, {2, 4}};
    vector<int> result1 = solution.shortestDistance(nums1, maxDiff1, queries1);
    for (auto& res : result1) cout << res << " ";
    cout << endl;

    vector<int> nums2 = {5, 3, 1, 9, 10};
    int maxDiff2 = 2;
    vector<vector<int>> queries2 = {{0, 1}, {0, 2}, {2, 3}, {4, 3}};
    vector<int> result2 = solution.shortestDistance(nums2, maxDiff2, queries2);
    for (auto& res : result2) cout << res << " ";
    cout << endl;

    vector<int> nums3 = {3, 6, 1};
    int maxDiff3 = 1;
    vector<vector<int>> queries3 = {{0, 0}, {0, 1}, {1, 2}};
    vector<int> result3 = solution.shortestDistance(nums3, maxDiff3, queries3);
    for (auto& res : result3) cout << res << " ";
    cout << endl;

    return 0;
}