#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string s;
    int pos;
    unordered_set<string> parseExpression() {
        unordered_set<string> res = parseTerm();
        while (pos < (int)s.size() && s[pos] == ',') {
            ++pos;
            unordered_set<string> term = parseTerm();
            res.insert(term.begin(), term.end());
        }
        return res;
    }
    
    unordered_set<string> parseTerm() {
        unordered_set<string> res = parseFactor();
        while (pos < (int)s.size() && (isalpha(s[pos]) || s[pos] == '{')) {
            unordered_set<string> nxt = parseFactor();
            unordered_set<string> combined;
            for (const string& a : res) {
                for (const string& b : nxt) {
                    combined.insert(a + b);
                }
            }
            res.swap(combined);
        }
        return res;
    }
    
    unordered_set<string> parseFactor() {
        if (isalpha(s[pos])) {
            unordered_set<string> st;
            st.insert(string(1, s[pos]));
            ++pos;
            return st;
        } else { // '{'
            ++pos; // skip '{'
            unordered_set<string> inner = parseExpression();
            ++pos; // skip '}'
            return inner;
        }
    }
    
    vector<string> braceExpansionII(string expression) {
        s = expression;
        pos = 0;
        unordered_set<string> resultSet = parseExpression();
        vector<string> result(resultSet.begin(), resultSet.end());
        sort(result.begin(), result.end());
        return result;
    }
};