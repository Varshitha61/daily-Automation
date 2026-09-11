#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> findEvenNumbers(vector<int>& digits) {
        vector<int> ans;
        int n = digits.size();
        for (int i = 0; i < n; ++i) {
            if (digits[i] == 0) continue; // leading zero not allowed
            for (int j = 0; j < n; ++j) {
                if (j == i) continue;
                for (int k = 0; k < n; ++k) {
                    if (k == i || k == j) continue;
                    if (digits[k] % 2 != 0) continue; // must be even
                    int num = digits[i] * 100 + digits[j] * 10 + digits[k];
                    ans.push_back(num);
                }
            }
        }
        sort(ans.begin(), ans.end());
        ans.erase(unique(ans.begin(), ans.end()), ans.end());
        return ans;
    }
};