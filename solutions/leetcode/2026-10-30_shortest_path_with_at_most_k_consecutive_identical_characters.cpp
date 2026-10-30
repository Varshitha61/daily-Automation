#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long minimumWeight(int n, vector<vector<int>>& edges, string labels, int k) {
        const long long INF = (1LL<<60);
        vector<vector<pair<int,int>>> adj(n);
        for (auto &e: edges) {
            int u=e[0], v=e[1], w=e[2];
            adj[u].push_back({v,w});
        }
        vector<vector<long long>> dist(n, vector<long long>(k+1, INF));
        using State = tuple<long long,int,int>; // dist, node, cnt
        priority_queue<State, vector<State>, greater<State>> pq;
        dist[0][1]=0;
        pq.emplace(0LL,0,1);
        while(!pq.empty()){
            auto [d,u,cnt]=pq.top(); pq.pop();
            if(d!=dist[u][cnt]) continue;
            if(u==n-1){
                // we could early exit but need minimal over cnt, continue
            }
            for(auto &pr: adj[u]){
                int v=pr.first, w=pr.second;
                int newcnt = (labels[v]==labels[u]) ? cnt+1 : 1;
                if(newcnt>k) continue;
                long long nd = d + w;
                if(nd < dist[v][newcnt]){
                    dist[v][newcnt]=nd;
                    pq.emplace(nd,v,newcnt);
                }
            }
        }
        long long ans=INF;
        for(int cnt=1;cnt<=k;++cnt) ans=min(ans, dist[n-1][cnt]);
        return ans==INF?-1:ans;
    }
};