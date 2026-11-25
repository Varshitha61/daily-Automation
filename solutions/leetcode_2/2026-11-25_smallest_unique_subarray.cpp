#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int smallestUniqueSubarray(vector<int>& nums) {
        int n = nums.size();
        const long long mod1 = 1000000007LL;
        const long long mod2 = 1000000009LL;
        const long long base = 91138233LL; // random large base
        
        vector<long long> h1(n+1,0), h2(n+1,0), p1(n+1,1), p2(n+1,1);
        for(int i=0;i<n;i++){
            h1[i+1] = (h1[i]*base + nums[i]) % mod1;
            h2[i+1] = (h2[i]*base + nums[i]) % mod2;
            p1[i+1] = (p1[i]*base) % mod1;
            p2[i+1] = (p2[i]*base) % mod2;
        }
        auto getHash = [&](int l,int r)->uint64_t{
            long long x1 = (h1[r] - h1[l]*p1[r-l]) % mod1;
            if(x1<0) x1+=mod1;
            long long x2 = (h2[r] - h2[l]*p2[r-l]) % mod2;
            if(x2<0) x2+=mod2;
            return ( (uint64_t)x1 << 32 ) | (uint64_t)x2;
        };
        auto existsUnique = [&](int L)->bool{
            unordered_map<uint64_t,int> cnt;
            cnt.reserve(n);
            for(int i=0;i+L<=n;i++){
                uint64_t h = getHash(i,i+L);
                ++cnt[h];
            }
            for(auto &kv:cnt){
                if(kv.second==1) return true;
            }
            return false;
        };
        int lo=1, hi=n, ans=n;
        while(lo<=hi){
            int mid = lo + (hi-lo)/2;
            if(existsUnique(mid)){
                ans=mid;
                hi=mid-1;
            }else lo=mid+1;
        }
        return ans;
    }
};