#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<long long> nums;
    long long x;
    // Read input as a line containing array like [0,2,1] or space separated numbers
    string s;
    if(!getline(cin, s)) return 0;
    // Remove brackets and commas
    for(char &c: s) if(c=='['||c==']'||c==',') c=' ';
    stringstream ss(s);
    while(ss>>x) nums.push_back(x);
    int n = nums.size();
    if(n==0){ cout<<-1; return 0; }
    int pos0 = -1;
    for(int i=0;i<n;i++) if(nums[i]==0){ pos0=i; break; }
    const int INF = 1e9;
    int ans = INF;
    // case 1: pure rotations
    bool ok = true;
    for(int i=0;i<n;i++){
        if(nums[(i+pos0)%n] != i){ ok=false; break; }
    }
    if(ok) ans = min(ans, pos0);
    // case 2: rotation then reverse (or reverse then rotation)
    int k = (pos0 + 1) % n;
    ok = true;
    for(int i=0;i<n;i++){
        int idx = (k + n - 1 - i) % n;
        if(nums[idx] != i){ ok=false; break; }
    }
    if(ok){
        int steps = 1 + min(k, n - k);
        ans = min(ans, steps);
    }
    if(ans==INF) cout<<-1;
    else cout<<ans;
    return 0;
}