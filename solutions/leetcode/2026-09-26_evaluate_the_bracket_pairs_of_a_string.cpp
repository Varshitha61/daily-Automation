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
        int n = s.size();
        for (int i = 0; i < n; ) {
            if (s[i] == '(') {
                int j = i + 1;
                while (j < n && s[j] != ')') ++j;
                string key = s.substr(i + 1, j - i - 1);
                auto it = mp.find(key);
                if (it != mp.end()) res += it->second;
                else res += '?';
                i = j + 1; // skip ')'
            } else {
                res += s[i];
                ++i;
            }
        }
        return res;
    }
};