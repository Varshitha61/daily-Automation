#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    if (!getline(cin, line)) return 0;
    vector<int> nums;
    {
        string num;
        for (char c : line) {
            if (isdigit(c) || c == '-') {
                num += c;
            } else {
                if (!num.empty()) {
                    nums.push_back(stoi(num));
                    num.clear();
                }
            }
        }
        if (!num.empty()) nums.push_back(stoi(num));
    }
    int target;
    if (!(cin >> target)) return 0;
    int lessCnt = 0, eqCnt = 0;
    for (int x : nums) {
        if (x < target) lessCnt++;
        else if (x == target) eqCnt++;
    }
    vector<int> res;
    for (int i = 0; i < eqCnt; ++i) res.push_back(lessCnt + i);
    cout << "[";
    for (size_t i = 0; i < res.size(); ++i) {
        if (i) cout << ",";
        cout << res[i];
    }
    cout << "]";
    return 0;
}