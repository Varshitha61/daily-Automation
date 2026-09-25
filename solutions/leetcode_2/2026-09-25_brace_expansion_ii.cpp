#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string s;
    int pos;
    
    set<string> parseExpression() {
        set<string> cur;
        cur.insert("");
        while (pos < (int)s.size() && s[pos] != '}' && s[pos] != ',') {
            set<string> term;
            if (isalpha(s[pos])) {
                term.insert(string(1, s[pos]));
                ++pos;
            } else if (s[pos] == '{') {
                ++pos; // skip '{'
                term = parseUnion();
            }
            set<string> nxt;
            for (const string& a : cur) {
                for (const string& b : term) {
                    nxt.insert(a + b);
                }
            }
            cur.swap(nxt);
        }
        return cur;
    }
    
    set<string> parseUnion() {
        set<string> res;
        while (true) {
            set<string> sub = parseExpression();
            res.insert(sub.begin(), sub.end());
            if (pos >= (int)s.size()) break;
            if (s[pos] == ',') {
                ++pos;
                continue;
            } else if (s[pos] == '}') {
                ++pos; // consume '}'
                break;
            }
        }
        return res;
    }
    
    vector<string> braceExpansionII(string expression) {
        s = expression;
        pos = 0;
        set<string> ans = parseExpression();
        return vector<string>(ans.begin(), ans.end());
    }
};