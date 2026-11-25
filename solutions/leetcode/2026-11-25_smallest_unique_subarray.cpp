#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<long long> nums;
    long long x;
    // Read entire input as numbers (LeetCode style: single line array)
    string s;
    if(!getline(cin, s)) return 0;
    // Extract numbers
    long long num = 0;
    bool inNum = false, neg = false;
    for(char c: s){
        if(c=='-'){
            neg = true;
        } else if(isdigit(c)){
            num = num*10 + (c-'0');
            inNum = true;
        } else {
            if(inNum){
                if(neg) num = -num;
                nums.push_back(num);
                num = 0; inNum = false; neg = false;
            }
        }
    }
    if(inNum){
        if(neg) num = -num;
        nums.push_back(num);
    }
    int n = nums.size();
    if(n==0) return 0;
    const uint64_t base = 1000003ULL; // random odd base
    const uint64_t offset = 1007ULL;
    vector<uint64_t> powBase(n+1);
    powBase[0]=1;
    for(int i=1;i<=n;i++) powBase[i]=powBase[i-1]*base;
    auto existsUnique = [&](int L)->bool{
        if(L>n) return false;
        unordered_map<uint64_t,int> cnt;
        cnt.reserve(n*2);
        uint64_t h=0;
        for(int i=0;i<L;i++) h = h*base + (uint64_t)(nums[i]+offset);
        cnt[h]++;
        uint64_t highPow = powBase[L-1];
        for(int i=L;i<n;i++){
            h = h - (uint64_t)(nums[i-L]+offset)*highPow;
            h = h*base + (uint64_t)(nums[i]+offset);
            cnt[h]++;
        }
        for(auto &p:cnt) if(p.second==1) return true;
        return false;
    };
    int lo=1, hi=n, ans=n;
    while(lo<=hi){
        int mid=(lo+hi)/2;
        if(existsUnique(mid)){
            ans=mid;
            hi=mid-1;
        }else lo=mid+1;
    }
    cout<<ans;
    return 0;
}