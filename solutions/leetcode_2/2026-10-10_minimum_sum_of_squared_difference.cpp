#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long minSumSquareDiff(vector<int>& nums1, vector<int>& nums2, int k1, int k2) {
        long long K = (long long)k1 + (long long)k2;
        const int MAXV = 200000; // enough for initial diffs up to 1e5
        vector<long long> freq(MAXV + 1, 0);
        long long totalDiff = 0;
        int curMax = 0;
        for (size_t i = 0; i < nums1.size(); ++i) {
            int d = abs(nums1[i] - nums2[i]);
            freq[d]++;
            totalDiff += d;
            if (d > curMax) curMax = d;
        }
        if (K >= totalDiff) return 0LL;
        int cur = curMax;
        while (K > 0 && cur > 0) {
            while (cur > 0 && freq[cur] == 0) --cur;
            if (cur == 0) break;
            long long cnt = freq[cur];
            int nxt = cur - 1;
            while (nxt > 0 && freq[nxt] == 0) --nxt;
            long long target = nxt; // could be 0
            long long stepsNeeded = (cur - target) * cnt;
            if (K >= stepsNeeded) {
                freq[target] += cnt;
                K -= stepsNeeded;
                freq[cur] = 0;
                cur = (int)target;
            } else {
                long long d = K / cnt; // full decrement for each
                long long r = K % cnt; // extra decrement for r elements
                int newVal = cur - (int)d;
                freq[cur] = 0;
                freq[newVal] += (cnt - r);
                if (newVal - 1 >= 0) freq[newVal - 1] += r;
                K = 0;
                break;
            }
        }
        long long ans = 0;
        for (int v = 0; v <= MAXV; ++v) {
            if (freq[v]) {
                ans += (long long)v * v * freq[v];
            }
        }
        return ans;
    }
};