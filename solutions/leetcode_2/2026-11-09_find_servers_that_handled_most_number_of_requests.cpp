#include <bits/stdc++.h>
using namespace std;

vector<long long> parseArray(const string& s) {
    vector<long long> res;
    string num;
    for (char c : s) {
        if (isdigit(c) || c == '-') num += c;
        else {
            if (!num.empty()) {
                res.push_back(stoll(num));
                num.clear();
            }
        }
    }
    if (!num.empty()) res.push_back(stoll(num));
    return res;
}

vector<int> solve(int k, const vector<long long>& arrival, const vector<long long>& load) {
    vector<int> cnt(k, 0);
    set<int> available;
    for (int i = 0; i < k; ++i) available.insert(i);
    using P = pair<long long, int>;
    priority_queue<P, vector<P>, greater<P>> busy; // (finish time, server id)

    int n = arrival.size();
    for (int i = 0; i < n; ++i) {
        long long cur = arrival[i];
        while (!busy.empty() && busy.top().first <= cur) {
            int id = busy.top().second;
            busy.pop();
            available.insert(id);
        }
        if (available.empty()) continue;
        int start = i % k;
        auto it = available.lower_bound(start);
        if (it == available.end()) it = available.begin();
        int server = *it;
        cnt[server]++;
        long long finish = cur + load[i];
        busy.emplace(finish, server);
        available.erase(it);
    }
    int mx = *max_element(cnt.begin(), cnt.end());
    vector<int> ans;
    for (int i = 0; i < k; ++i) if (cnt[i] == mx) ans.push_back(i);
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    while (true) {
        // read k
        int k;
        while (getline(cin, line)) {
            if (!line.empty() && line.find_first_not_of(" \t\r\n") != string::npos) break;
        }
        if (cin.eof() && line.empty()) break;
        if (line.empty()) break;
        k = stoi(line);
        string arrLine, loadLine;
        if (!getline(cin, arrLine)) break;
        if (!getline(cin, loadLine)) break;
        vector<long long> arrival = parseArray(arrLine);
        vector<long long> load = parseArray(loadLine);
        vector<int> ans = solve(k, arrival, load);
        cout << '[';
        for (size_t i = 0; i < ans.size(); ++i) {
            if (i) cout << ',';
            cout << ans[i];
        }
        cout << ']' << '\n';
    }
    return 0;
}