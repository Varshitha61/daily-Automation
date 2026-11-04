#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<long long> nums;
    long long x, k;
    // Read input as per the example format: array line, then x, then k
    // We'll read until EOF
    string line;
    if (!getline(cin, line)) return 0;
    // parse array
    {
        stringstream ss(line);
        char ch;
        ss >> ch; // '['
        long long val;
        while (ss >> val) {
            nums.push_back(val);
            ss >> ch; // ',' or ']'
            if (ch == ']') break;
        }
    }
    if (!(cin >> x)) return 0;
    if (!(cin >> k)) return 0;
    int n = nums.size();
    int X = (int)x;
    int K = (int)k;
    int m = n - X + 1;
    vector<long long> cost(m, 0);
    // sliding window median with cost
    multiset<long long> low, high;
    long long sumLow = 0, sumHigh = 0;
    int needLow = (X + 1) / 2;
    auto rebalance = [&]() {
        while ((int)low.size() > needLow) {
            auto it = prev(low.end());
            long long v = *it;
            low.erase(it);
            sumLow -= v;
            high.insert(v);
            sumHigh += v;
        }
        while ((int)low.size() < needLow) {
            auto it = high.begin();
            long long v = *it;
            high.erase(it);
            sumHigh -= v;
            low.insert(v);
            sumLow += v;
        }
        while (!low.empty() && !high.empty() && *prev(low.end()) > *high.begin()) {
            long long a = *prev(low.end());
            long long b = *high.begin();
            // remove a from low
            auto itLow = prev(low.end());
            low.erase(itLow);
            sumLow -= a;
            // remove b from high
            auto itHigh = high.begin();
            high.erase(itHigh);
            sumHigh -= b;
            // insert swapped
            low.insert(b);
            sumLow += b;
            high.insert(a);
            sumHigh += a;
        }
    };
    auto add = [&](long long v) {
        if (low.empty() || v <= *prev(low.end())) {
            low.insert(v);
            sumLow += v;
        } else {
            high.insert(v);
            sumHigh += v;
        }
        rebalance();
    };
    auto remove = [&](long long v) {
        auto itLow = low.find(v);
        if (itLow != low.end()) {
            low.erase(itLow);
            sumLow -= v;
        } else {
            auto itHigh = high.find(v);
            if (itHigh != high.end()) {
                high.erase(itHigh);
                sumHigh -= v;
            }
        }
        rebalance();
    };
    // init first window
    for (int i = 0; i < X; ++i) add(nums[i]);
    for (int start = 0; start <= n - X; ++start) {
        long long median = *prev(low.end());
        long long szLow = low.size();
        long long szHigh = high.size();
        cost[start] = median * szLow - sumLow + sumHigh - median * szHigh;
        if (start + X < n) {
            remove(nums[start]);
            add(nums[start + X]);
        }
    }
    const long long INF = (1LL<<60);
    vector<vector<long long>> dp(n+1, vector<long long>(K+1, INF));
    dp[0][0] = 0;
    for (int pos = 0; pos <= n; ++pos) {
        for (int cnt = 0; cnt <= K; ++cnt) {
            long long cur = dp[pos][cnt];
            if (cur == INF) continue;
            if (pos < n) {
                dp[pos+1][cnt] = min(dp[pos+1][cnt], cur);
            }
            if (cnt < K && pos <= n - X) {
                dp[pos + X][cnt+1] = min(dp[pos + X][cnt+1], cur + cost[pos]);
            }
        }
    }
    long long ans = INF;
    for (int pos = 0; pos <= n; ++pos) ans = min(ans, dp[pos][K]);
    cout << ans << "\n";
    return 0;
}