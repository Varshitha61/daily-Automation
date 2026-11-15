#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int numberOfSubarrays(vector<int>& arr) {
        const long long MOD = 1000000007LL;
        long long even = 1, odd = 0;
        int parity = 0;
        for (int x : arr) {
            parity ^= (x & 1);
            if (parity == 0) ++even;
            else ++odd;
        }
        long long ans = (even % MOD) * (odd % MOD) % MOD;
        return (int)ans;
    }
};