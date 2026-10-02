#include <bits/stdc++.h>
using namespace std;

void backtrack(int n, int open, int close, string &cur, vector<string> &res) {
    if ((int)cur.size() == 2 * n) {
        res.push_back(cur);
        return;
    }
    if (open < n) {
        cur.push_back('(');
        backtrack(n, open + 1, close, cur, res);
        cur.pop_back();
    }
    if (close < open) {
        cur.push_back(')');
        backtrack(n, open, close + 1, cur, res);
        cur.pop_back();
    }
}

vector<string> generateParenthesis(int n) {
    vector<string> res;
    string cur;
    backtrack(n, 0, 0, cur, res);
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<int> inputs;
    int x;
    while (cin >> x) inputs.push_back(x);
    for (size_t i = 0; i < inputs.size(); ++i) {
        int n = inputs[i];
        vector<string> ans = generateParenthesis(n);
        cout << "[";
        for (size_t j = 0; j < ans.size(); ++j) {
            cout << "\"" << ans[j] << "\"";
            if (j + 1 < ans.size()) cout << ",";
        }
        cout << "]";
        if (i + 1 < inputs.size()) cout << "\n";
    }
    return 0;
}