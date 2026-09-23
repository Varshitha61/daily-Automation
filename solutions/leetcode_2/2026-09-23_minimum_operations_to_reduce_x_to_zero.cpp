#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    vector<long long> nums;
    long long x;
    // read array line
    while (getline(cin, line)) {
        if (!line.empty()) break;
    }
    for (char &c : line) {
        if (c == '[' || c == ']') c = ' ';
        else if (c == ',') c = ' ';
    }
    stringstream ss(line);
    long long v;
    while (ss >> v) nums.push_back(v);
    // read x line
    while (getline(cin, line)) {
        if (!line.empty()) break;
    }
    stringstream ss2(line);
    ss2 >> x;

    long long total = 0;
    for (auto val : nums) total += val;
    long long target = total - x;
    if (target < 0) {
        cout << -1;
        return 0;
    }
    if (target == 0) {
        cout << nums.size();
        return 0;
    }
    int n = nums.size();
    long long sum = 0;
    int left = 0;
    int maxlen = -1;
    for (int right = 0; right < n; ++right) {
        sum += nums[right];
        while (sum > target && left <= right) {
            sum -= nums[left];
            ++left;
        }
        if (sum == target) {
            maxlen = max(maxlen, right - left + 1);
        }
    }
    if (maxlen == -1) cout << -1;
    else cout << (n - maxlen);
    return 0;
}