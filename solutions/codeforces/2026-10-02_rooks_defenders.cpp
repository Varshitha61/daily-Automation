#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<int> bit;
    Fenwick(int n=0): n(n), bit(n+1,0) {}
    void add(int idx, int val){
        for(; idx<=n; idx+=idx&-idx) bit[idx]+=val;
    }
    int sumPrefix(int idx) const{
        int res=0;
        for(; idx>0; idx-=idx&-idx) res+=bit[idx];
        return res;
    }
    int rangeSum(int l, int r) const{
        if(l>r) return 0;
        return sumPrefix(r)-sumPrefix(l-1);
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,q;
    if(!(cin>>n>>q)) return 0;
    vector<int> rowCnt(n+1,0), colCnt(n+1,0);
    Fenwick rowZero(n), colZero(n);
    for(int i=1;i<=n;i++){
        rowZero.add(i,1);
        colZero.add(i,1);
    }
    while(q--){
        int t; cin>>t;
        if(t==1){
            int x,y; cin>>x>>y;
            if(++rowCnt[x]==1) rowZero.add(x,-1);
            if(++colCnt[y]==1) colZero.add(y,-1);
        }else if(t==2){
            int x,y; cin>>x>>y;
            if(--rowCnt[x]==0) rowZero.add(x,1);
            if(--colCnt[y]==0) colZero.add(y,1);
        }else if(t==3){
            int x1,y1,x2,y2; cin>>x1>>y1>>x2>>y2;
            int emptyRows = rowZero.rangeSum(x1,x2);
            int emptyCols = colZero.rangeSum(y1,y2);
            if(emptyRows==0 || emptyCols==0) cout<<"Yes\n";
            else cout<<"No\n";
        }
    }
    return 0;
}