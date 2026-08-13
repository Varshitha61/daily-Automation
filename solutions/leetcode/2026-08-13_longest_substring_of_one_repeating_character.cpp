#include <iostream>
#include <vector>
#include <string>

using namespace std;

int longestSubstringAfterQuery(string& s, char c, int index) {
    int n = s.size();
    s[index] = c;
    int maxLength = 0;
    for (int i = 0; i < n; i++) {
        int count = 0;
        char currChar = s[i];
        while (i < n && s[i] == currChar) {
            count++;
            i++;
        }
        maxLength = max(maxLength, count);
        i--;
    }
    return maxLength;
}

vector<int> longestSubstring(vector<string> queries, string s) {
    vector<int> result;
    for (auto& query : queries) {
        int index = stoi(query.substr(1));
        char c = query[0];
        result.push_back(longestSubstringAfterQuery(s, c, index));
    }
    return result;
}

int main() {
    string s = "babacc";
    string queryCharacters = "bcb";
    vector<int> queryIndices = {1, 3, 3};
    vector<string> queries;
    for (int i = 0; i < queryCharacters.size(); i++) {
        queries.push_back(queryCharacters.substr(i, 1) + to_string(queryIndices[i]));
    }
    vector<int> result = longestSubstring(queries, s);
    for (int i = 0; i < result.size(); i++) {
        cout << result[i] << " ";
    }
    return 0;
}