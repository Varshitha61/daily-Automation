#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isValid(const string& str) {
        int bal = 0;
        for (char c : str) {
            if (c == '(') bal++;
            else if (c == ')') {
                if (bal == 0) return false;
                bal--;
            }
        }
        return bal == 0;
    }
    
    vector<string> removeInvalidParentheses(string s) {
        vector<string> result;
        unordered_set<string> visited;
        queue<string> q;
        q.push(s);
        visited.insert(s);
        bool found = false;
        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; ++i) {
                string cur = q.front(); q.pop();
                if (isValid(cur)) {
                    result.push_back(cur);
                    found = true;
                }
                if (found) continue;
                for (int j = 0; j < (int)cur.size(); ++j) {
                    if (cur[j] != '(' && cur[j] != ')') continue;
                    string nxt = cur.substr(0, j) + cur.substr(j + 1);
                    if (!visited.count(nxt)) {
                        visited.insert(nxt);
                        q.push(nxt);
                    }
                }
            }
            if (found) break;
        }
        if (result.empty()) result.push_back("");
        return result;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    while (getline(cin, s)) {
        if (s.empty()) continue;
        Solution sol;
        vector<string> ans = sol.removeInvalidParentheses(s);
        for (size_t i = 0; i < ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }
    return 0;
}