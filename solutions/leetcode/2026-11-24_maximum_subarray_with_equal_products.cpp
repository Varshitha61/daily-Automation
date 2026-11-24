#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestSubarray(vector<int>& nums) {
        int n = nums.size();
        // primes up to 10: 2,3,5,7
        const int primes[4] = {2,3,5,7};
        int mask[11];
        mask[0]=0;
        for(int x=1;x<=10;x++){
            int m=0;
            for(int i=0;i<4;i++){
                if(x%primes[i]==0) m|=(1<<i);
            }
            mask[x]=m;
        }
        int best=0;
        for(int i=0;i<n;i++){
            for(int j=i;j<n;j++){
                int g=0;
                for(int k=i;k<=j;k++) g=std::gcd(g, nums[k]);
                int used=0;
                bool ok=true;
                for(int k=i;k<=j;k++){
                    int reduced = nums[k]/g;
                    int m = mask[reduced];
                    if(used & m){ ok=false; break; }
                    used |= m;
                }
                if(ok) best = max(best, j-i+1);
            }
        }
        return best;
    }
};