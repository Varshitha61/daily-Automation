#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<long long> bit;
    Fenwick(int n=0): n(n), bit(n+1,0) {}
    void add(int idx, long long val){
        for(; idx<=n; idx+=idx&-idx) bit[idx]+=val;
    }
    long long sumPrefix(int idx) const{
        long long s=0;
        for(; idx>0; idx-=idx&-idx) s+=bit[idx];
        return s;
    }
    // smallest idx such that prefix sum >= k (k>=1)
    int kth(long long k) const{
        int idx=0;
        int mask=1;
        while(mask<<1 <= n) mask <<=1;
        for(int d=mask; d>0; d>>=1){
            int next = idx + d;
            if(next<=n && bit[next] < k){
                idx = next;
                k -= bit[next];
            }
        }
        return idx+1;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<long long> nums;
    long long x,k;
    // read input as per given format (three lines)
    // first line: array like [5,-2,1,3,7,3,6,4,-1]
    string line;
    if(!getline(cin,line)) return 0;
    // parse numbers
    long long num=0; bool neg=false, innum=false;
    for(char c: line){
        if(c=='-'){neg=true; innum=true; num=0;}
        else if(isdigit(c)){
            num = num*10 + (c-'0');
            innum=true;
        }else{
            if(innum){
                nums.push_back(neg?-num:num);
                num=0; neg=false; innum=false;
            }
        }
    }
    if(innum) nums.push_back(neg?-num:num);
    // second line x
    if(!getline(cin,line)) return 0;
    x = stoll(line);
    // third line k
    if(!getline(cin,line)) return 0;
    k = stoll(line);
    int n = nums.size();
    int X = (int)x;
    int K = (int)k;
    int m = n - X + 1;
    vector<long long> cost(m);
    // coordinate compression
    vector<long long> vals = nums;
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int M = vals.size();
    Fenwick fenCnt(M), fenSum(M);
    long long totalSum=0;
    auto idxOf = [&](long long v){
        return (int)(lower_bound(vals.begin(), vals.end(), v) - vals.begin()) + 1; // 1-indexed
    };
    for(int i=0;i<n;i++){
        int id = idxOf(nums[i]);
        fenCnt.add(id,1);
        fenSum.add(id,nums[i]);
        totalSum+=nums[i];
        if(i>=X){
            int idr = idxOf(nums[i-X]);
            fenCnt.add(idr,-1);
            fenSum.add(idr,-nums[i-X]);
            totalSum-=nums[i-X];
        }
        if(i>=X-1){
            int start = i - X + 1;
            int need = (X+1)/2;
            int medIdx = fenCnt.kth(need);
            long long median = vals[medIdx-1];
            long long cntLeft = fenCnt.sumPrefix(medIdx);
            long long sumLeft = fenSum.sumPrefix(medIdx);
            long long cntRight = X - cntLeft;
            long long sumRight = totalSum - sumLeft;
            long long c = median*cntLeft - sumLeft + sumRight - median*cntRight;
            cost[start]=c;
        }
    }
    const long long INF = (1LL<<60);
    vector<long long> dpPrev(m,0), dpCurr(m,INF);
    // dpPrev is for t=0 (cost 0)
    for(int t=1;t<=K;t++){
        fill(dpCurr.begin(), dpCurr.end(), INF);
        for(int i=0;i<m;i++){
            if(i>0) dpCurr[i]=min(dpCurr[i], dpCurr[i-1]); // not take
            long long prev;
            if(i - X >= 0) prev = dpPrev[i - X];
            else prev = (t==1 ? 0 : INF);
            if(prev!=INF){
                dpCurr[i]=min(dpCurr[i], prev + cost[i]);
            }
        }
        dpPrev.swap(dpCurr);
    }
    cout << dpPrev[m-1] << "\n";
    return 0;
}