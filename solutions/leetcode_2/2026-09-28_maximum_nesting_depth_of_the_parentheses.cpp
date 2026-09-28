#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    if (!getline(cin, s)) return 0;
    // Remove possible surrounding quotes if present
    if (!s.empty() && s.front() == '"' && s.back() == '"') {
        s = s.substr(1, s.size() - 2);
    }
    int cur = 0, mx = 0;
    for (char c : s) {
        if (c == '(') {
            ++cur;
            mx = max(mx, cur);
        } else if (c == ')') {
            --cur;
        }
    }
    cout << mx;
    return 0;
}