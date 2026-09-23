#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string line;
    if (!getline(cin, line)) return 0;
    // Remove brackets and spaces
    for (char &c : line) {
        if (c == '[' || c == ']' ) c = ' ';
    }
    stringstream ss(line);
    vector<long long> nums;
    string token;
    while (getline(ss, token, ',')) {
        stringstream ts(token);
        long long val;
        if (ts >> val) nums.push_back(val);
    }
    long long x;
    if (!(cin >> x)) return 0;
    
    int n = nums.size();
    long long total = 0;
    for (long long v : nums) total += v;
    long long target = total - x;
    if (target < 0) {
        cout << -1;
        return 0;
    }
    if (target == 0) {
        cout << n;
        return 0;
    }
    int left = 0;
    long long cur = 0;
    int maxlen = -1;
    for (int right = 0; right < n; ++right) {
        cur += nums[right];
        while (cur > target && left <= right) {
            cur -= nums[left];
            ++left;
        }
        if (cur == target) {
            maxlen = max(maxlen, right - left + 1);
        }
    }
    if (maxlen == -1) {
        cout << -1;
    } else {
        cout << (n - maxlen);
    }
    return 0;
}