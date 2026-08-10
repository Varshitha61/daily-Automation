#include <iostream>
#include <vector>
#include <cmath>

class Solution {
public:
    bool winnerSquareGame(int n) {
        std::vector<bool> dp(n + 1, false);
        for (int i = 1; i <= n; i++) {
            dp[i] = false;
            for (int j = 1; j * j <= i; j++) {
                if (!dp[i - j * j]) {
                    dp[i] = true;
                    break;
                }
            }
        }
        return dp[n];
    }
};

int main() {
    Solution solution;
    std::cout << std::boolalpha << solution.winnerSquareGame(1) << std::endl;  // true
    std::cout << std::boolalpha << solution.winnerSquareGame(2) << std::endl;  // false
    std::cout << std::boolalpha << solution.winnerSquareGame(4) << std::endl;  // true
    return 0;
}