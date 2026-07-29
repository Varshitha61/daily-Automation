#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    string getSmallestString(string s, int k) {
        int n = s.size();
        vector<int> count(26, 0);
        for (char c : s) {
            count[c - 'a']++;
        }
        vector<char> firstHalf;
        vector<char> secondHalf;
        char midChar = '\0';
        for (int i = 0; i < 26; i++) {
            if (count[i] > 0) {
                if (count[i] % 2 == 1) {
                    if (midChar != '\0') {
                        return "";
                    }
                    midChar = 'a' + i;
                }
                for (int j = 0; j < count[i] / 2; j++) {
                    firstHalf.push_back('a' + i);
                }
            }
        }
        sort(firstHalf.begin(), firstHalf.end());
        int index = 0;
        for (int i = 0; i < firstHalf.size(); i++) {
            if (k > 1) {
                int j = i + 1;
                while (j < firstHalf.size() && firstHalf[j] == firstHalf[i]) {
                    j++;
                }
                int countSame = j - i;
                long long numPermutations = permutationCount(countSame, index);
                if (k > numPermutations) {
                    k -= numPermutations;
                    index += countSame;
                    i = j - 1;
                } else {
                    break;
                }
            }
        }
        if (k == 1) {
            string result = "";
            for (int i = 0; i < firstHalf.size(); i++) {
                if (i == index) {
                    result += firstHalf[i];
                    i++;
                }
                if (i < firstHalf.size()) {
                    result += firstHalf[i];
                }
            }
            if (midChar != '\0') {
                result += midChar;
            }
            for (int i = firstHalf.size() - 1; i >= 0; i--) {
                result += firstHalf[i];
            }
            return result;
        } else {
            return "";
        }
    }

    long long permutationCount(int countSame, int index) {
        if (countSame == 0) {
            return 1;
        }
        long long result = 1;
        for (int i = countSame; i > 0; i--) {
            result *= i;
            if (index + i == countSame) {
                result /= factorial(countSame - index);
            }
        }
        return result;
    }

    long long factorial(int n) {
        long long result = 1;
        for (int i = 2; i <= n; i++) {
            result *= i;
        }
        return result;
    }
};

int main() {
    Solution solution;
    cout << solution.getSmallestString("abba", 2) << endl;  // Output: "baab"
    cout << solution.getSmallestString("aa", 2) << endl;    // Output: ""
    cout << solution.getSmallestString("bacab", 1) << endl; // Output: "abcba"
    return 0;
}