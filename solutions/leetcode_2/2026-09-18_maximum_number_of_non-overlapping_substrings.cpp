#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<string> maxNumOfSubstrings(string s) {
        int n = s.size();
        const int INF = 1e9;
        vector<int> first(26, INF), last(26, -1);
        for (int i = 0; i < n; ++i) {
            int c = s[i] - 'a';
            first[c] = min(first[c], i);
            last[c] = max(last[c], i);
        }
        vector<pair<int,int>> intervals;
        for (int c = 0; c < 26; ++c) {
            if (first[c] == INF) continue;
            int l = first[c];
            int r = last[c];
            bool ok = true;
            for (int i = l; i <= r; ++i) {
                int ch = s[i] - 'a';
                if (first[ch] < l) { ok = false; break; }
                r = max(r, last[ch]);
            }
            if (ok) intervals.emplace_back(l, r);
        }
        sort(intervals.begin(), intervals.end(),
             [](const pair<int,int>& a, const pair<int,int>& b){
                 return a.second < b.second;
             });
        vector<string> res;
        int prevEnd = -1;
        for (auto &p : intervals) {
            if (p.first > prevEnd) {
                res.push_back(s.substr(p.first, p.second - p.first + 1));
                prevEnd = p.second;
            }
        }
        return res;
    }
};