#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    if(!(cin >> s)) return 0;
    int k;
    cin >> k;
    vector<pair<char,char>> pairs(k);
    for(int i=0;i<k;i++){
        string p; cin >> p;
        pairs[i] = {p[0], p[1]};
    }
    long long ans = 0;
    for(auto [a,b]: pairs){
        int cntA = 0, cntB = 0;
        for(char ch: s){
            if(ch==a) cntA++;
            else if(ch==b) cntB++;
            else{
                ans += min(cntA, cntB);
                cntA = cntB = 0;
            }
        }
        ans += min(cntA, cntB);
    }
    cout << ans << "\n";
    return 0;
}