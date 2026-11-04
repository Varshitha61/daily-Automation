#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000007;

long long modpow(long long a, long long e) {
    long long r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    const int MAXN = 1000;
    vector<long long> fact(MAXN+1,1), invfact(MAXN+1,1);
    for(int i=1;i<=MAXN;i++) fact[i]=fact[i-1]*i%MOD;
    invfact[MAXN]=modpow(fact[MAXN], MOD-2);
    for(int i=MAXN;i>0;i--) invfact[i-1]=invfact[i]*i%MOD;
    auto C = [&](int n,int k)->long long{
        if(k<0||k>n) return 0;
        return fact[n]*invfact[k]%MOD*invfact[n-k]%MOD;
    };
    while(T--){
        int n,k;
        cin>>n>>k;
        vector<int>a(n);
        for(int &x:a)cin>>x;
        vector<int>b=a;
        sort(b.begin(), b.end(), greater<int>());
        int threshold = b[k-1];
        int cnt_total=0,cnt_needed=0;
        for(int x:a) if(x==threshold) cnt_total++;
        for(int i=0;i<k;i++) if(b[i]==threshold) cnt_needed++;
        cout<<C(cnt_total, cnt_needed)<<"\n";
    }
    return 0;
}