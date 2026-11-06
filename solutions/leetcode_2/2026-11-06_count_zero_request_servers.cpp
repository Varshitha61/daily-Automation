#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> countServers(int n, vector<vector<int>>& logs, int x, vector<int>& queries) {
        int m = logs.size();
        vector<pair<int,int>> arr;
        arr.reserve(m);
        for (auto &lg : logs) {
            arr.emplace_back(lg[1], lg[0]-1); // (time, server)
        }
        sort(arr.begin(), arr.end()); // sort by time
        
        int qn = queries.size();
        vector<pair<int,int>> qvec;
        qvec.reserve(qn);
        for (int i = 0; i < qn; ++i) qvec.emplace_back(queries[i], i);
        sort(qvec.begin(), qvec.end());
        
        vector<int> cnt(n, 0);
        int zeroCount = n;
        vector<int> ans(qn);
        int L = 0, R = 0;
        for (auto &qr : qvec) {
            int q = qr.first;
            int idx = qr.second;
            // add logs with time <= q
            while (R < m && arr[R].first <= q) {
                int srv = arr[R].second;
                if (cnt[srv] == 0) zeroCount--;
                cnt[srv]++;
                ++R;
            }
            // remove logs with time < q - x
            int low = q - x;
            while (L < m && arr[L].first < low) {
                int srv = arr[L].second;
                cnt[srv]--;
                if (cnt[srv] == 0) zeroCount++;
                ++L;
            }
            ans[idx] = zeroCount;
        }
        return ans;
    }
};