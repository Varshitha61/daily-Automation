#include <bits/stdc++.h>
using namespace std;

class Fenwick {
    vector<int> bit;
    int n;
public:
    Fenwick(int n): n(n), bit(n+1,0) {}
    void add(int idx, int val){
        for(; idx<=n; idx+=idx&-idx) bit[idx]+=val;
    }
    int sumPrefix(int idx){
        int res=0;
        for(; idx>0; idx-=idx&-idx) res+=bit[idx];
        return res;
    }
    int rangeSum(int l, int r){
        if(l>r) return 0;
        if(l<1) l=1;
        if(r<1) return 0;
        if(r>n) r=n;
        return sumPrefix(r)-sumPrefix(l-1);
    }
};

class Solution {
public:
    vector<int> countServers(int n, vector<vector<int>>& logs, int x, vector<int>& queries) {
        // sort logs by time
        sort(logs.begin(), logs.end(), [](const vector<int>& a, const vector<int>& b){
            return a[1] < b[1];
        });
        int maxTime = 0;
        for (auto &lg: logs) maxTime = max(maxTime, lg[1]);
        for (int q: queries) maxTime = max(maxTime, q);
        Fenwick ft(maxTime+2);
        vector<int> latest(n+1, 0);
        int m = logs.size();
        int ptr = 0;
        int Q = queries.size();
        vector<pair<int,int>> qvec;
        qvec.reserve(Q);
        for(int i=0;i<Q;i++) qvec.emplace_back(queries[i], i);
        sort(qvec.begin(), qvec.end());
        vector<int> ans(Q);
        for(auto &pr: qvec){
            int q = pr.first;
            int idx = pr.second;
            while(ptr < m && logs[ptr][1] <= q){
                int server = logs[ptr][0];
                int t = logs[ptr][1];
                int prev = latest[server];
                if(prev>0) ft.add(prev, -1);
                latest[server] = t;
                ft.add(t, 1);
                ++ptr;
            }
            int l = q - x;
            if(l<1) l=1;
            int cnt = ft.rangeSum(l, q);
            ans[idx] = n - cnt;
        }
        return ans;
    }
};