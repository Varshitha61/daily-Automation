#include <bits/stdc++.h>
using namespace std;

struct NodeState{
    int v, p, d;
    int fe, fo;
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin>>n)) return 0;
    vector<vector<int>> adj(n+1);
    for(int i=0;i<n-1;i++){
        int u,v;cin>>u>>v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<int> init(n+1), goal(n+1);
    for(int i=1;i<=n;i++) cin>>init[i];
    for(int i=1;i<=n;i++) cin>>goal[i];
    vector<int> ans;
    vector<NodeState> st;
    st.reserve(n);
    st.push_back({1,0,0,0,0});
    while(!st.empty()){
        NodeState cur = st.back(); st.pop_back();
        int v=cur.v, p=cur.p, d=cur.d, fe=cur.fe, fo=cur.fo;
        int curVal = init[v];
        if(d%2==0) curVal ^= fe;
        else curVal ^= fo;
        int newFe=fe, newFo=fo;
        if(curVal!=goal[v]){
            ans.push_back(v);
            if(d%2==0) newFe ^= 1;
            else newFo ^= 1;
        }
        for(int u: adj[v]){
            if(u==p) continue;
            st.push_back({u, v, d+1, newFe, newFo});
        }
    }
    cout<<ans.size()<<"\n";
    for(int x: ans) cout<<x<<"\n";
    return 0;
}