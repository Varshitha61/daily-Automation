#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

struct SegTree {
    struct Node {
        long long sum = 0;      // sum of values
        long long sumSq = 0;    // sum of squares
        long long lazy = 0;     // pending addition
    };
    int n;
    vector<Node> st;
    SegTree(int n): n(n) {
        st.resize(4*n);
    }
    void apply(int p, int l, int r, long long v) {
        long long len = r - l + 1;
        st[p].sumSq = (st[p].sumSq + 2LL * v % MOD * st[p].sum % MOD + v * v % MOD * len % MOD) % MOD;
        st[p].sum = (st[p].sum + v % MOD * len) % MOD;
        st[p].lazy = (st[p].lazy + v) % MOD;
    }
    void push(int p, int l, int r) {
        if (st[p].lazy != 0 && l != r) {
            int m = (l + r) >> 1;
            apply(p<<1, l, m, st[p].lazy);
            apply(p<<1|1, m+1, r, st[p].lazy);
            st[p].lazy = 0;
        }
    }
    void rangeAdd(int L, int R, long long v) { rangeAdd(1,0,n-1,L,R,v); }
    void rangeAdd(int p, int l, int r, int L, int R, long long v) {
        if (R < l || r < L) return;
        if (L <= l && r <= R) {
            apply(p,l,r,v);
            return;
        }
        push(p,l,r);
        int m = (l+r)>>1;
        rangeAdd(p<<1,l,m,L,R,v);
        rangeAdd(p<<1|1,m+1,r,L,R,v);
        st[p].sum = (st[p<<1].sum + st[p<<1|1].sum) % MOD;
        st[p].sumSq = (st[p<<1].sumSq + st[p<<1|1].sumSq) % MOD;
    }
    long long querySumSq() { return st[1].sumSq; }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<int> nums;
    int x;
    // Read input as per LeetCode style: first line may be like "nums = [1,2,1]"
    // We'll parse all integers from stdin.
    string token;
    while (cin >> token) {
        size_t pos = 0;
        while (pos < token.size()) {
            if (isdigit(token[pos]) || (token[pos]=='-' && pos+1<token.size() && isdigit(token[pos+1]))) {
                int sign = 1;
                if (token[pos]=='-') { sign=-1; ++pos; }
                long long val = 0;
                while (pos<token.size() && isdigit(token[pos])) {
                    val = val*10 + (token[pos]-'0');
                    ++pos;
                }
                nums.push_back((int)(sign*val));
            } else ++pos;
        }
    }
    int n = nums.size();
    if (n==0) { cout << 0 << "\n"; return 0; }
    const int MAXV = 100000;
    vector<int> lastPos(MAXV+1, -1);
    SegTree seg(n);
    long long ans = 0;
    for (int r=0;r<n;++r){
        int val = nums[r];
        int prev = lastPos[val];
        int L = prev + 1;
        int R = r;
        if (L <= R) seg.rangeAdd(L,R,1);
        ans += seg.querySumSq();
        if (ans>=MOD) ans%=MOD;
        lastPos[val]=r;
    }
    cout << (ans%MOD) << "\n";
    return 0;
}