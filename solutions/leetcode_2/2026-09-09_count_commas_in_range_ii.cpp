#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long countCommas(long long n) {
        static unsigned long long pow10[20];
        pow10[0] = 1;
        for (int i = 1; i < 20; ++i) pow10[i] = pow10[i-1] * 10ULL;
        unsigned long long total = 0;
        for (int d = 1; pow10[d-1] <= (unsigned long long)n; ++d) {
            unsigned long long start = pow10[d-1];
            unsigned long long end = min<unsigned long long>((unsigned long long)n, pow10[d]-1);
            unsigned long long cnt = end - start + 1;
            unsigned long long commasPerNumber = (d - 1) / 3;
            total += cnt * commasPerNumber;
        }
        return (long long)total;
    }
};