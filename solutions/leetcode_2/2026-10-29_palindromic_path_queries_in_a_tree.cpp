#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    struct SegTree {
        int n;
        vector<int> tree;
        SegTree(int n=0){init(n);}
        void init(int n_){
            n=n_;
            tree.assign(4*n,0);
        }
        void build(int idx,int l,int r, const vector<int>& arr){
            if(l==r){ tree[idx]=arr[l]; return;}
            int m=(l+r)/2;
            build(idx*2,l,m,arr);
            build(idx*2+1,m+1,r,arr);
            tree[idx]=tree[idx*2]^tree[idx*2+1];
        }
        void build(const vector<int>& arr){ build(1,0,n-1,arr); }
        void update(int idx,int l,int r,int pos,int val){
            if(l==r){ tree[idx]=val; return;}
            int m=(l+r)/2;
            if(pos<=m) update(idx*2,l,m,pos,val);
            else update(idx*2+1,m+1,r,pos,val);
            tree[idx]=tree[idx*2]^tree[idx*2+1];
        }
        void update(int pos,int val){ update(1,0,n-1,pos,val); }
        int query(int idx,int l,int r,int ql,int qr){
            if(ql>r||qr<l) return 0;
            if(ql<=l && r<=qr) return tree[idx];
            int m=(l+r)/2;
            return query(idx*2,l,m,ql,qr) ^ query(idx*2+1,m+1,r,ql,qr);
        }
        int query(int l,int r){ return query(1,0,n-1,l,r); }
    };
    
    int n;
    vector<vector<int>> adj;
    vector<int> parent, depth, heavy, head, pos, sz;
    int curPos;
    vector<int> baseMask;
    SegTree seg;
    
    int dfs(int v,int p){
        parent[v]=p;
        sz[v]=1;
        int maxSub=0;
        for(int to:adj[v]){
            if(to==p) continue;
            depth[to]=depth[v]+1;
            int sub=dfs(to,v);
            sz[v]+=sub;
            if(sub>maxSub){
                maxSub=sub;
                heavy[v]=to;
            }
        }
        return sz[v];
    }
    void decompose(int v,int h){
        head[v]=h;
        pos[v]=curPos++;
        baseMask[pos[v]] = 1 << (s_global[v]-'a');
        if(heavy[v]!=-1) decompose(heavy[v],h);
        for(int to:adj[v]){
            if(to==parent[v]||to==heavy[v]) continue;
            decompose(to,to);
        }
    }
    string s_global;
    
    int queryPath(int a,int b){
        int res=0;
        while(head[a]!=head[b]){
            if(depth[head[a]]<depth[head[b]]) swap(a,b);
            int h=head[a];
            res ^= seg.query(pos[h],pos[a]);
            a=parent[h];
        }
        if(depth[a]>depth[b]) swap(a,b);
        res ^= seg.query(pos[a],pos[b]);
        return res;
    }
    
    vector<bool> palindromePathQueries(int n_, vector<vector<int>>& edges, string s, vector<string>& queries) {
        n=n_;
        s_global=s;
        adj.assign(n, {});
        for(auto &e:edges){
            int u=e[0], v=e[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        parent.assign(n, -1);
        depth.assign(n,0);
        heavy.assign(n,-1);
        head.assign(n,0);
        pos.assign(n,0);
        sz.assign(n,0);
        curPos=0;
        baseMask.assign(n,0);
        dfs(0,-1);
        decompose(0,0);
        seg.init(n);
        seg.build(baseMask);
        vector<bool> ans;
        for(const string& q:queries){
            stringstream ss(q);
            string type; ss>>type;
            if(type=="update"){
                int u; char c; ss>>u>>c;
                s_global[u]=c;
                seg.update(pos[u], 1 << (c-'a'));
            }else{ // query
                int u,v; ss>>u>>v;
                int mask=queryPath(u,v);
                bool ok = (mask==0) || ((mask & (mask-1))==0);
                ans.push_back(ok);
            }
        }
        return ans;
    }
};