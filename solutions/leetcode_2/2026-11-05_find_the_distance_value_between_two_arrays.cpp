#include <bits/stdc++.h>
using namespace std;

vector<int> parseArray(const string& s) {
    vector<int> res;
    string num;
    for (char c : s) {
        if (c == '-' || isdigit(c)) {
            num += c;
        } else {
            if (!num.empty()) {
                res.push_back(stoi(num));
                num.clear();
            }
        }
    }
    if (!num.empty()) res.push_back(stoi(num));
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line1, line2, line3;
    if (!getline(cin, line1)) return 0;
    if (!getline(cin, line2)) return 0;
    if (!getline(cin, line3)) return 0;
    vector<int> arr1 = parseArray(line1);
    vector<int> arr2 = parseArray(line2);
    int d = stoi(line3);
    sort(arr2.begin(), arr2.end());
    int count = 0;
    for (int x : arr1) {
        int left = x - d;
        int right = x + d;
        auto it = lower_bound(arr2.begin(), arr2.end(), left);
        bool ok = true;
        if (it != arr2.end() && *it <= right) ok = false;
        if (ok) count++;
    }
    cout << count;
    return 0;
}