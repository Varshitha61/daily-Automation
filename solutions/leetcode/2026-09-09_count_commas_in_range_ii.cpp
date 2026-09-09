#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long countCommas(long long n) {
        __int128 total = 0;
        long long pow10 = 1;
        for (int d = 1; d <= 16; ++d) {
            long long start = pow10;
            if (start > n) break;
            long long nextPow = (d == 16) ? LLONG_MAX : pow10 * 10;
            long long end = min(n, nextPow - 1);
            long long cnt = end - start + 1;
            long long commasPer = (d - 1) / 3;
            total += (__int128)cnt * commasPer;
            pow10 = nextPow;
        }
        return (long long)total;
    }
};