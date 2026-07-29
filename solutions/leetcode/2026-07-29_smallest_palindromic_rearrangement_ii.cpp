#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Solution {
public:
    string getSmallestString(string s, int k) {
        int n = s.size();
        vector<int> cnt(26, 0);
        for (char c : s) {
            cnt[c - 'a']++;
        }
        vector<char> firstHalf;
        vector<char> secondHalf;
        char mid = '\0';
        for (int i = 0; i < 26; i++) {
            if (cnt[i] % 2 == 1) {
                if (mid != '\0') {
                    return "";
                }
                mid = 'a' + i;
            }
            for (int j = 0; j < cnt[i] / 2; j++) {
                firstHalf.push_back('a' + i);
            }
        }
        sort(firstHalf.begin(), firstHalf.end());
        int m = firstHalf.size();
        vector<vector<int>> factorial(m + 1, vector<int>(m + 1, 0));
        for (int i = 0; i <= m; i++) {
            factorial[i][0] = 1;
            factorial[i][i] = 1;
            for (int j = 1; j < i; j++) {
                factorial[i][j] = factorial[i - 1][j - 1] + factorial[i - 1][j];
            }
        }
        vector<bool> used(m, false);
        string result = "";
        k--;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                if (used[j]) {
                    continue;
                }
                int remaining = m - i - 1;
                int count = 0;
                for (int l = j + 1; l < m; l++) {
                    if (!used[l]) {
                        count++;
                    }
                }
                if (factorial[remaining][count] > k) {
                    result += firstHalf[j];
                    used[j] = true;
                    break;
                } else {
                    k -= factorial[remaining][count];
                }
            }
        }
        if (mid != '\0') {
            result += mid;
        }
        reverse(firstHalf.begin(), firstHalf.end());
        result += firstHalf;
        return result;
    }
};

int main() {
    Solution solution;
    string s;
    int k;
    cin >> s >> k;
    cout << solution.getSmallestString(s, k) << endl;
    return 0;
}