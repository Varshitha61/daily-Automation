#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> assignTasks(vector<int>& servers, vector<int>& tasks) {
        int n = servers.size(), m = tasks.size();
        vector<int> ans(m);
        using Server = pair<int,int>; // weight, index
        using Busy = tuple<long long,int,int>; // freeTime, weight, index
        
        priority_queue<Server, vector<Server>, greater<Server>> freeServers;
        priority_queue<Busy, vector<Busy>, greater<Busy>> busyServers;
        
        for (int i = 0; i < n; ++i) freeServers.emplace(servers[i], i);
        
        long long curTime = 0;
        for (int i = 0; i < m; ++i) {
            curTime = max<long long>(curTime, i);
            // release servers that have become free by curTime
            while (!busyServers.empty() && get<0>(busyServers.top()) <= curTime) {
                auto [ft, w, idx] = busyServers.top(); busyServers.pop();
                freeServers.emplace(w, idx);
            }
            // if no free server, jump to next free time
            if (freeServers.empty()) {
                auto [ft, w, idx] = busyServers.top(); busyServers.pop();
                curTime = ft;
                freeServers.emplace(w, idx);
                while (!busyServers.empty() && get<0>(busyServers.top()) <= curTime) {
                    auto [ft2, w2, idx2] = busyServers.top(); busyServers.pop();
                    freeServers.emplace(w2, idx2);
                }
            }
            // assign task i
            auto [w, idx] = freeServers.top(); freeServers.pop();
            ans[i] = idx;
            busyServers.emplace(curTime + tasks[i], w, idx);
        }
        return ans;
    }
};