#include <iostream>
#include <string>
#include <unordered_set>

class Solution {
public:
    std::string smallestSubsequence(std::string s) {
        int n = s.length();
        std::unordered_set<char> visited;
        std::unordered_map<char, int> lastOccurrence;
        for (int i = 0; i < n; i++) {
            lastOccurrence[s[i]] = i;
        }
        std::string result;
        for (int i = 0; i < n; i++) {
            if (visited.find(s[i]) != visited.end()) {
                continue;
            }
            while (!result.empty() && result.back() > s[i] && lastOccurrence[result.back()] > i) {
                visited.erase(result.back());
                result.pop_back();
            }
            result.push_back(s[i]);
            visited.insert(s[i]);
        }
        return result;
    }
};

int main() {
    Solution solution;
    std::cout << solution.smallestSubsequence("bcabc") << std::endl;  // Output: "abc"
    std::cout << solution.smallestSubsequence("cbacdcbc") << std::endl;  // Output: "acdb"
    return 0;
}