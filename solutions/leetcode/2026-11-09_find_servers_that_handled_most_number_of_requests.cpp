#include <bits/stdc++.h>
using namespace std;

vector<long long> parseInts(const string& s) {
    vector<long long> res;
    long long num = 0;
    bool inNum = false;
    bool neg = false;
    for (char c : s) {
        if (c == '-' && !inNum) {
            neg = true;
        } else if (isdigit(c)) {
            num = num * 10 + (c - '0');
            inNum = true;
        } else {
            if (inNum) {
                res.push_back(neg ? -num : num);
                num = 0;
                inNum = false;
                neg = false;
            }
        }
    }
    if (inNum) res.push_back(neg ? -num : num);
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long k_ll;
    if (!(cin >> k_ll)) return 0;
    int k = (int)k_ll;
    string line;
    getline(cin, line); // consume endline
    
    // read arrival line
    if (!getline(cin, line)) return 0;
    vector<long long> arrival = parseInts(line);
    // read load line
    if (!getline(cin, line)) return 0;
    vector<long long> load = parseInts(line);
    
    int n = arrival.size();
    vector<int> cnt(k, 0);
    set<int> available;
    for (int i = 0; i < k; ++i) available.insert(i);
    using P = pair<long long,int>; // finish time, server id
    priority_queue<P, vector<P>, greater<P>> busy;
    
    for (int i = 0; i < n; ++i) {
        long long curTime = arrival[i];
        // free servers
        while (!busy.empty() && busy.top().first <= curTime) {
            int sid = busy.top().second;
            busy.pop();
            available.insert(sid);
        }
        if (available.empty()) continue; // drop
        int start = i % k;
        auto it = available.lower_bound(start);
        if (it == available.end()) it = available.begin();
        int sid = *it;
        // assign
        cnt[sid]++;
        available.erase(it);
        busy.emplace(curTime + load[i], sid);
    }
    
    int mx = 0;
    for (int c : cnt) mx = max(mx, c);
    vector<int> ans;
    for (int i = 0; i < k; ++i) if (cnt[i] == mx) ans.push_back(i);
    
    // output in [a,b,c] format
    cout << '[';
    for (size_t i = 0; i < ans.size(); ++i) {
        if (i) cout << ',';
        cout << ans[i];
    }
    cout << ']';
    return 0;
}