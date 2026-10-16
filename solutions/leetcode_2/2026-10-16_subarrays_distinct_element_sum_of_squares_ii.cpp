#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

struct BIT {
    int n;
    vector<long long> b1, b2;
    BIT(int n): n(n), b1(n+2,0), b2(n+2,0) {}
    void add(vector<long long>& bit, int idx, long long val){
        for(; idx<=n; idx+=idx&-idx){
            bit[idx] += val;
            if(bit[idx]>=MOD) bit[idx]-=MOD;
        }
    }
    long long sum(const vector<long long>& bit, int idx) const{
        long long res=0;
        for(; idx>0; idx-=idx&-idx){
            res += bit[idx];
            if(res>=MOD) res-=MOD;
        }
        return res;
    }
    // add val to [l,r] (1-indexed)
    void rangeAdd(int l,int r,long long val){
        if(l>r) return;
        val %= MOD;
        add(b1,l,val