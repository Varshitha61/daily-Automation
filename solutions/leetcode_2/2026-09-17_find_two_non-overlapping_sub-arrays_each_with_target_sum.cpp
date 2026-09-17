#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minSumOfLengths(const vector<int>& arr, int target) {
        const int INF = 1e9;
        int n = arr.size();
        vector<int> best(n, INF);
        long long sum = 0;
        int left = 0;
        int ans = INF;
        for (int right = 0; right < n; ++right) {
            sum += arr[right];
            while (sum > target && left <= right) {
                sum -= arr[left];
                ++left;
            }
            if (sum == target) {
                int len = right - left + 1;
                if (left > 0 && best[left - 1] != INF) {
                    ans = min(ans, best[left - 1] + len);
                }
                best[right] = min((right > 0 ? best[right - 1] : INF), len);
            } else {
                best[right] = (right > 0 ? best[right - 1] : INF);
            }
        }
        return ans == INF ? -1 : ans;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    // Read array line
    while (getline(cin, line)) {
        if (!line.empty()) break;
    }
    if (line.empty()) return 0;
    for (char &c : line) {
        if (!(isdigit(c) || c == '-' )) c = ' ';
    }
    stringstream ss(line);
    vector<int> arr;
    int val;
    while (ss >> val) arr.push_back(val);
    // Read target line
    string tline;
    while (getline(cin, tline)) {
        if (!tline.empty()) break;
    }
    int target = 0;
    {
        for (char &c : tline) {
            if (!(isdigit(c) || c == '-')) c = ' ';
        }
        stringstream ts(tline);
        ts >> target;
    }
    Solution sol;
    int result = sol.minSumOfLengths(arr, target);
    cout << result;
    return 0;
}