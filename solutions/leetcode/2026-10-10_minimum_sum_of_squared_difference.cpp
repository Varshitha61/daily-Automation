#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long minSumSquareDiff(vector<int>& nums1, vector<int>& nums2, int k1, int k2) {
        int n = nums1.size();
        vector<long long> d(n);
        for (int i = 0; i < n; ++i) d[i] = llabs((long long)nums1[i] - (long long)nums2[i]);
        sort(d.begin(), d.end(), greater<long long>());
        long long K = (long long)k1 + (long long)k2;
        size_t i = 0;
        while (i < d.size() && K > 0) {
            long long cur = d[i];
            size_t j = i;
            while (j < d.size() && d[j] == cur) ++j;
            long long cnt = (long long)j; // number of elements >= cur
            long long next = (j < d.size()) ? d[j] : 0;
            long long diff = cur - next;
            long long need = cnt * diff;
            if (K >= need) {
                for (size_t p = 0; p < cnt; ++p) d[p] = next;
                K -= need;
                i = j;
            } else {
                long long dec = K / cnt;
                long long rem = K % cnt;
                long long new_val = cur - dec;
                for (size_t p = 0; p < cnt; ++p) {
                    if (p < (size_t)rem) d[p] = new_val - 1;
                    else d[p] = new_val;
                }
                K = 0;
                break;
            }
        }
        long long ans = 0;
        if (K > 0) {
            // all differences are zero now
            ans = (K % 2 == 0) ? 0 : 1;
        } else {
            for (long long x : d) ans += x * x;
        }
        return ans;
    }
};