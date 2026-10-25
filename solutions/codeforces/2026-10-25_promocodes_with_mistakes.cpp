#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if(!(cin >> n)) return 0;
    vector<string> a(n);
    for(int i=0;i<n;i++) cin >> a[i];
    if(n==1){
        cout << 6 << "\n";
        return 0;
    }
    int minDist = 7; // max possible 6
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            int d=0;
            for(int k=0;k<6;k++) if(a[i][k]!=a[j][k]) d++;
            if(d<minDist) minDist=d;
            if(minDist==1){ // cannot get lower
                cout << 0 << "\n";
                return 0;
            }
        }
    }
    int ans = (minDist - 1) / 2;
    cout << ans << "\n";
    return 0;
}