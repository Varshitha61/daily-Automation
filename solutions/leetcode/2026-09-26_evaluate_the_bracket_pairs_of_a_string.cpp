#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string evaluate(string s, vector<vector<string>>& knowledge) {
        unordered_map<string, string> mp;
        mp.reserve(knowledge.size() * 2);
        for (auto &kv : knowledge) {
            mp[kv[0]] = kv[1];
        }
        string res;
        res.reserve(s.size());
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == '(') {
                size_t j = i + 1;
                while (j < s.size() && s[j] != ')') ++j;
                string key = s.substr(i + 1, j - i - 1);
                auto it = mp.find(key);
                if (it != mp.end()) res += it->second;
                else res += '?';
                i = j; // skip to ')'
            } else {
                res += s[i];
            }
        }
        return res;
    }
};