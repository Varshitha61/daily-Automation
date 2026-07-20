#include <vector>

class Solution {
public:
    std::vector<std::vector<int>> shiftGrid(std::vector<std::vector<int>>& grid, int k) {
        int m = grid.size();
        int n = grid[0].size();
        int total = m * n;
        k %= total;
        
        std::vector<std::vector<int>> result(m, std::vector<int>(n));
        
        for (int i = 0; i < total; i++) {
            int oldRow = i / n;
            int oldCol = i % n;
            int newRow = (i + k) % total / n;
            int newCol = (i + k) % total % n;
            result[newRow][newCol] = grid[oldRow][oldCol];
        }
        
        return result;
    }
};