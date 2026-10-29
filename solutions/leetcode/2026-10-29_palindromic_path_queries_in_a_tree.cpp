#include <bits/stdc++.h>
using namespace std;

class Fenwick {
    int n;
    vector<int> bit;
public:
    Fenwick(int n=0): n(n), bit(n+2,0) {}
    void init(int n_) { n=n_; bit.assign(n+2,0); }
    void add(int idx, int val){
        for(; idx<=n; idx+=idx&-idx) bit[idx] ^= val;
    }
    // range xor [l,r]
    void rangeXor(int l, int r, int val){
        if(l>r) return;
        add(l, val);
        if(r+1<=n) add(r+1, val);
    }
    int pointQuery(int idx){
        int res=0;
        for(; idx>0; idx-=idx&-idx) res ^= bit[idx];
        return res;
    }
};

class Solution {
public:
    vector<bool> palindromePathQueries(int n, vector<vector<int>>& edges, string s, vector<string>& queries) {
        vector<vector<int>> g(n);
        for(auto &e: edges){
            int u=e[0], v=e[1];
            g[u].push_back(v);
            g[v].push_back(u);
        }
        const int LOG = 17;
        vector<array<int, LOG>> up(n);
        vector<int> depth(n), tin(n), tout(n);
        int timer=1;
        function<void(int,int)> dfs = [&](int v,int p){
            up[v][0]=p==-1?v:p;
            for(int k=1;k<LOG;k++) up[v][k]=up[up[v][k-1]][k-1];
            tin[v]=timer++;
            for(int to:g[v]){
                if(to==p) continue;
                depth[to]=depth[v]+1;
                dfs(to,v);
            }
            tout[v]=timer-1;
        };
        dfs(0,-1);
        auto lca = [&](int a,int b){
            if(depth[a]<depth[b]) swap(a,b);
            int diff=depth[a]-depth[b];
            for(int k=0;k<LOG;k++) if(diff>>k &1) a=up[a][k];
            if(a==b) return a;
            for(int k=LOG-1;k>=0;k--){
                if(up[a][k]!=up[b][k]){
                    a=up[a][k];
                    b=up[b][k];
                }
            }
            return up[a][0];
        };
        vector<int> curMask(n);
        for(int i=0;i<n;i++) curMask[i]=1<<(s[i]-'a');
        Fenwick bit(n);
        for(int i=0;i<n;i++) bit.rangeXor(tin[i], tout[i], curMask[i]);
        vector<bool> ans;
        for(const string& qs: queries){
            stringstream ss(qs);
            string type; ss>>type;
            if(type=="update"){
                int node; char c; ss>>node>>c;
                int newMask=1<<(c-'a');
                int diff=curMask[node]^newMask;
                if(diff){
                    bit.rangeXor(tin[node], tout[node], diff);
                    curMask[node]=newMask;
                }
            }else{ // query
                int u,v; ss>>u>>v;
                int w=lca(u,v);
                int pu=bit.pointQuery(tin[u]);
                int pv=bit.pointQuery(tin[v]);
                int pathMask=pu^pv^curMask[w];
                ans.push_back( (pathMask & (pathMask-1)) == 0 );
            }
        }
        return ans;
    }
};