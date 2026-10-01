#include <bits/stdc++.h>
using namespace std;

bool isValid(const string& s) {
    stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else {
            if (st.empty()) return false;
            char top = st.top(); st.pop();
            if ((c == ')' && top != '(') ||
                (c == ']' && top != '[') ||
                (c == '}' && top != '{')) return false;
        }
    }
    return st.empty();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    if (!getline(cin, line)) return 0;
    // Remove possible surrounding quotes and whitespace
    while (!line.empty() && isspace(line.back())) line.pop_back();
    while (!line.empty() && isspace(line.front())) line.erase(line.begin());
    if (line.size() >= 2 && ((line.front() == '"' && line.back() == '"') || (line.front() == '\'' && line.back() == '\''))) {
        line = line.substr(1, line.size() - 2);
    }
    bool ans = isValid(line);
    cout << (ans ? "true" : "false");
    return 0;
}