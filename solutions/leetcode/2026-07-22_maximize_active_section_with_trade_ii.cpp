#include <iostream>
#include <vector>
#include <string>

using namespace std;

int countActiveSections(const string& s) {
    int count = 0;
    bool prev = false;
    for (char c : s) {
        if (c == '1' && !prev) {
            count++;
        }
        prev = (c == '1');
    }
    return count;
}

vector<int> maxActiveSections(string s, vector<vector<int>>& queries) {
    vector<int> results;
    for (auto& query : queries) {
        int left = query[0];
        int right = query[1];
        string substring = s.substr(left, right - left + 1);
        substring = "1" + substring + "1";
        int maxCount = countActiveSections(substring);
        int n = substring.size();
        for (int i = 1; i < n - 1; i++) {
            if (substring[i] == '1' && substring[i - 1] == '0' && substring[i + 1] == '0') {
                int j = i;
                while (j < n - 1 && substring[j + 1] == '1') {
                    j++;
                }
                string newSubstring = substring;
                for (int k = i; k <= j; k++) {
                    newSubstring[k] = '0';
                }
                int k = i - 1;
                while (k >= 1 && newSubstring[k] == '0') {
                    k--;
                }
                int end = k + 1;
                while (end < n - 1 && newSubstring[end] == '0') {
                    end++;
                }
                for (int m = k + 1; m <= end; m++) {
                    newSubstring[m] = '1';
                }
                maxCount = max(maxCount, countActiveSections(newSubstring.substr(1, newSubstring.size() - 2)));
            }
        }
        results.push_back(maxCount);
    }
    return results;
}

int main() {
    string s;
    int q;
    cin >> s >> q;
    vector<vector<int>> queries(q, vector<int>(2));
    for (int i = 0; i < q; i++) {
        cin >> queries[i][0] >> queries[i][1];
    }
    vector<int> results = maxActiveSections(s, queries);
    for (int result : results) {
        cout << result << " ";
    }
    return 0;
}