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
        res.reserve(s.size() * 2);
        int n = s.size();
        for (int i = 0; i < n; ++i) {
            if (s[i] != '(') {
                res.push_back(s[i]);
            } else {
                int j = i + 1;
                while (j < n && s[j] != ')') ++j;
                string key = s.substr(i + 1, j - i - 1);
                auto it = mp.find(key);
                if (it != mp.end()) res += it->second;
                else res.push_back('?');
                i = j; // move to closing ')'
            }
        }
        return res;
    }
};