#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string input, line;
    while (getline(cin, line)) {
        input += line;
    }
    vector<vector<int>> grid;
    vector<int> cur;
    int depth = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (c == '[') {
            depth++;
            if (depth == 2) cur.clear();
        } else if (c == ']') {
            if (depth == 2) {
                grid.push_back(cur);
                cur.clear();
            }
            depth--;
        } else if (isdigit(c)) {
            int val = 0;
            while (i < input.size() && isdigit(input[i])) {
                val = val * 10 + (input[i] - '0');
                ++i;
            }
            --i; // step back because for-loop will increment
            cur.push_back(val);
        }
    }
    if (grid.empty()) return 0;
    int m = grid.size();
    int n = grid[0].size();
    vector<int> rowCnt(m, 0), colCnt(n, 0);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (grid[i][j] == 1) {
                rowCnt[i]++;
                colCnt[j]++;
            }
    int ans = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            if (grid[i][j] == 1 && (rowCnt[i] > 1 || colCnt[j] > 1))
                ++ans;
    cout << ans;
    return 0;
}