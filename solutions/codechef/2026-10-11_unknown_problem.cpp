#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    vector<int> data;
    int x;
    while (cin >> x) data.push_back(x);
    if (data.empty()) return 0;
    
    size_t pos = 0;
    int T = data[pos++];
    bool possible = true;
    size_t tempPos = pos;
    for (int i = 0; i < T; ++i) {
        if (tempPos >= data.size()) { possible = false; break; }
        int N = data[tempPos++];
        if (N < 0) { possible = false; break; }
        if (tempPos + N > data.size()) { possible = false; break; }
        tempPos += N;
    }
    if (!possible || tempPos != data.size()) {
        // treat as single test case
        T = 1;
        pos = 0;
    } else {
        // parsing as T succeeded, reset pos to after T
        pos = 1;
    }
    
    for (int tc = 0; tc < T; ++tc) {
        if (pos >= data.size()) break;
        int N = data[pos++];
        int even = 0, odd = 0;
        for (int i = 0; i < N && pos < data.size(); ++i, ++pos) {
            if (data[pos] % 2 == 0) ++even;
            else ++odd;
        }
        if (even > odd) cout << "READY FOR BATTLE";
        else cout << "NOT READY";
        if (tc != T - 1) cout << '\n';
    }
    return 0;
}