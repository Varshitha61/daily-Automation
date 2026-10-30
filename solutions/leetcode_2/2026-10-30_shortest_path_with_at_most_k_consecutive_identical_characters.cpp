#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin >> n)) return 0;
    string dummy;
    getline(cin, dummy); // consume endline
    string edgesLine;
    getline(cin, edgesLine);
    // parse numbers
    vector<int> nums;
    long long cur = 0;
    bool inNum = false;
    for(char ch: edgesLine){
        if(ch=='-' || isdigit(ch)){
            cur = cur*10 + (ch-'0');
            inNum = true;
        } else {
            if(inNum){
                nums.push_back((int)cur);
                cur = 0;
                inNum = false;
            }
        }
    }
    if(inNum) nums.push_back((int)cur);
    vector<vector<pair<int,int>>> adj(n);
    for(size_t i=0;i+2<nums.size();i+=3){
        int u=nums[i], v=nums[i+1], w=nums[i+2];
        adj[u].push_back({v,w});
    }
    string labelLine;
    getline(cin, labelLine);
    // remove spaces and quotes
    string labels;
    for(char ch: labelLine){
        if(ch!=' ' && ch!='\"' && ch!='\'')
            labels.push_back(ch);
    }
    int k;
    cin >> k;
    const long long INF = (1LL<<60);
    vector<vector<long long>> dist(n, vector<long long>(k+1, INF));
    using State = tuple<long long,int,int>; // dist, node, run
    priority_queue<State, vector<State>, greater<State>> pq;
    dist[0][1]=0;
    pq.emplace(0LL,0,1);
    while(!pq.empty()){
        auto [d,u,run]=pq.top(); pq.pop();
        if(d!=dist[u][run]) continue;
        for(auto [v,w]: adj[u]){
            int newRun = (labels[v]==labels[u]) ? run+1 : 1;
            if(newRun>k) continue;
            long long nd = d + w;
            if(nd < dist[v][newRun]){
                dist[v][newRun]=nd;
                pq.emplace(nd,v,newRun);
            }
        }
    }
    long long ans=INF;
    for(int r=1;r<=k;r++) ans=min(ans, dist[n-1][r]);
    if(ans==INF) cout << -1 << "\n";
    else cout << ans << "\n";
    return 0;
}