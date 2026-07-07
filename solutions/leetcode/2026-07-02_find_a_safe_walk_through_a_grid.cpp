#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

struct Node {
    int x, y, health;
    Node(int x, int y, int health) : x(x), y(y), health(health) {}
};

struct Compare {
    bool operator()(const Node& a, const Node& b) {
        return a.health > b.health;
    }
};

class Solution {
public:
    bool isSafe(vector<vector<int>>& grid, int x, int y) {
        return x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size();
    }

    bool dfs(vector<vector<int>>& grid, int x, int y, int health, unordered_set<string>& visited) {
        if (x == grid.size() - 1 && y == grid[0].size() - 1) return true;
        string key = to_string(x) + "," + to_string(y);
        if (visited.find(key) != visited.end()) return false;
        visited.insert(key);
        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (auto& dir : directions) {
            int nx = x + dir.first, ny = y + dir.second;
            if (isSafe(grid, nx, ny) && health - grid[nx][ny] > 0) {
                if (dfs(grid, nx, ny, health - grid[nx][ny], visited)) return true;
            }
        }
        return false;
    }

    bool isPossibleToCutPath(vector<vector<int>>& grid, int health) {
        unordered_set<string> visited;
        return dfs(grid, 0, 0, health, visited);
    }
};

int main() {
    Solution solution;
    vector<vector<int>> grid = {{0,1,0,0,0},{0,1,0,1,0},{0,0,0,1,0}};
    int health = 1;
    cout << boolalpha << solution.isPossibleToCutPath(grid, health) << endl;
    return 0;
}