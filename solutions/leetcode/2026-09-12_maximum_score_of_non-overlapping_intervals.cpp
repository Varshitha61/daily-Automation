#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    struct State {
        long long sum;
        int len;
        int idx[4];
        State(long long s = -1, int l = 0) : sum(s), len(l) {
            for (int i = 0; i < 4; ++i) idx[i] = 0;
        }
    };
    
    static bool better(const State& a, const State& b) {
        if (a.sum != b.sum) return a.sum > b.sum;
        int la = a.len, lb = b.len;
        int m = min(la, lb);
        for (int i = 0; i < m; ++i) {
            if (a.idx[i] != b.idx[i]) return a.idx[i] < b.idx[i];
        }
        return la < lb;
    }
    
    vector<int> maxScoreIndices(vector<vector<int>>& intervals) {
        int n = intervals.size();
        struct Item {int l, r; long long w; int id;};
        vector<Item> a;
        a.reserve(n);
        for (int i = 0; i < n; ++i) {
            a.push_back({intervals[i][0], intervals[i][1], intervals[i][2], i});
        }
        sort(a.begin(), a.end(), [](const Item& x, const Item& y){
            if (x.r != y.r) return x.r < y.r;
            return x.l < y.l;
        });
        vector<int> ends(n);
        for (int i = 0; i < n; ++i) ends[i] = a[i].r;
        
        // dp[i][c]: best using first i intervals (0..i-1), with exactly c intervals selected
        vector<array<State,5>> dp(n+1);
        dp[0][0] = State(0,0);
        for (int c = 1; c <= 4; ++c) dp[0][c] = State(-1,0);
        
        for (int i = 1; i <= n; ++i) {
            // copy not taking i-1 interval
            for (int c = 0; c <= 4; ++c) dp[i][c] = dp[i-1][c];
            // find p: number of intervals with end < start_i
            int start_i = a[i-1].l;
            int p = upper_bound(ends.begin(), ends.end(), start_i - 1) - ends.begin(); // count
            for (int c = 1; c <= 4; ++c) {
                const State& prev = dp[p][c-1];
                if (prev.sum < 0) continue;
                State cand = prev;
                cand.sum += a[i-1].w;
                cand.idx[cand.len] = a[i-1].id;
                cand.len += 1;
                if (better(cand, dp[i][c])) dp[i][c] = cand;
            }
        }
        State best = State(-1,0);
        for (int c = 0; c <= 4; ++c) {
            if (better(dp[n][c], best)) best = dp[n][c];
        }
        vector<int> res(best.len);
        for (int i = 0; i < best.len; ++i) res[i] = best.idx[i];
        sort(res.begin(), res.end()); // ensure increasing order as typical output
        return res;
    }
};