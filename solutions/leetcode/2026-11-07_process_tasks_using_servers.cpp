#include <bits/stdc++.h>
using namespace std;

struct Server {
    int weight;
    int index;
};

struct Busy {
    long long freeTime;
    int weight;
    int index;
};

struct CmpAvail {
    bool operator()(const Server& a, const Server& b) const {
        if (a.weight != b.weight) return a.weight > b.weight;
        return a.index > b.index;
    }
};

struct CmpBusy {
    bool operator()(const Busy& a, const Busy& b) const {
        if (a.freeTime != b.freeTime) return a.freeTime > b.freeTime;
        if (a.weight != b.weight) return a.weight > b.weight;
        return a.index > b.index;
    }
};

class Solution {
public:
    vector<int> assignTasks(vector<int>& servers, vector<int>& tasks) {
        int n = servers.size(), m = tasks.size();
        priority_queue<Server, vector<Server>, CmpAvail> avail;
        priority_queue<Busy, vector<Busy>, CmpBusy> busy;
        for (int i = 0; i < n; ++i) avail.push({servers[i], i});
        vector<int> ans(m);
        long long cur = 0;
        for (int i = 0; i < m; ++i) {
            cur = max<long long>(cur, i);
            while (!busy.empty() && busy.top().freeTime <= cur) {
                auto b = busy.top(); busy.pop();
                avail.push({b.weight, b.index});
            }
            if (avail.empty()) {
                auto b = busy.top(); busy.pop();
                cur = b.freeTime;
                avail.push({b.weight, b.index});
                while (!busy.empty() && busy.top().freeTime == cur) {
                    auto b2 = busy.top(); busy.pop();
                    avail.push({b2.weight, b2.index});
                }
            }
            auto s = avail.top(); avail.pop();
            ans[i] = s.index;
            busy.push({cur + tasks[i], s.weight, s.index});
        }
        return ans;
    }
};