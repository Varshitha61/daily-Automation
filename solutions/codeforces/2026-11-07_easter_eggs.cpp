#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin>>n)) return 0;
    string base = "ROYGBIV";
    string extra = "GBIV";
    string res = base;
    int idx = 0;
    while ((int)res.size() < n) {
        res.push_back(extra[idx%4]);
        ++idx;
    }
    cout << res.substr(0, n) << "\n";
    return 0;
}