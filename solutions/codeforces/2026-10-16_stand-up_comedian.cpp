#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin>>T)) return 0;
    while(T--){
        long long a1,a2,a3,a4;
        cin>>a1>>a2>>a3>>a4;
        long long mn = min(a2,a3);
        long long base = a1 + 2*mn;
        bool diff = (a2!=a3);
        if(diff && a1>0) base += 1;
        long long minMood = a1;
        if(diff && a1>0) minMood = a1-1;
        long long safe4 = min(a4, minMood);
        long long safe_used = base + safe4;
        long long total = a1+a2+a3+a4;
        long long ans = safe_used + (total > safe_used ? 1 : 0);
        cout<<ans<<"\n";
    }
    return 0;
}