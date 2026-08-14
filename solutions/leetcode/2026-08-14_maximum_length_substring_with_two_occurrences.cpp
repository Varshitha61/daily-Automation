#include <iostream>
#include <string>
#include <unordered_map>

class Solution {
public:
    int max_length(std::string s) {
        int max_len = 0;
        for (int i = 0; i < s.length(); i++) {
            std::unordered_map<char, int> count;
            for (int j = i; j < s.length(); j++) {
                count[s[j]]++;
                if (count[s[j]] > 2) {
                    break;
                }
                max_len = std::max(max_len, j - i + 1);
            }
        }
        return max_len;
    }
};

int main() {
    Solution solution;
    std::cout << solution.max_length("bcbbbcba") << std::endl;  // Output: 4
    std::cout << solution.max_length("aaaa") << std::endl;      // Output: 2
    return 0;
}