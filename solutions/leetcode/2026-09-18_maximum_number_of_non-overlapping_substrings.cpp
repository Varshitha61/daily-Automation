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
            if (first[c] == INF) continue; // character not present
            int l = first[c], r = last[c];
            int nl = l, nr = r;
            bool changed = true;
            while (changed) {
                changed = false;
                nl = l; nr = r;
                for (int i = nl; i <= nr; ++i) {
                    int ch = s[i] - 'a';
                    if (first[ch] < l) { l = first[ch]; changed = true; }
                    if (last[ch] > r) { r = last[ch]; changed = true; }
                }
            }
            if (l == first[c]) { // canonical interval
                intervals.emplace_back(l, r);
            }
        }
        sort(intervals.begin(), intervals.end(), [](const pair<int,int>& a, const pair<int,int>& b){
            if (a.second != b.second) return a.second < b.second;
            return a.first < b.first;
        });
        vector<string> ans;
        int prevEnd = -1;
        for (auto &p : intervals) {
            if (p.first > prevEnd) {
                ans.push_back(s.substr(p.first, p.second - p.first + 1));
                prevEnd = p.second;
            }
        }
        return ans;
    }
};