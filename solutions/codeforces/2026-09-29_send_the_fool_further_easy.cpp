#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin >> n)) return 0;
    vector<vector<pair<int,int>>> adj(n);
    for(int i=0;i<n-1;i++){
        int u,v,c;
        cin>>u>>v>>c;
        adj[u].push_back({v,c});
        adj[v].push_back({u,c});
    }
    long long ans=0;
    vector<bool> vis(n,false);
    function<void(int,long long)> dfs = [&](int u,long long dist){
        vis[u]=true;
        ans = max(ans, dist);
        for(auto [v,w]: adj[u]){
            if(!vis[v]){
                dfs(v, dist + w);
            }
        }
    };
    dfs(0,0);
    cout << ans << "\n";
    return 0;
}