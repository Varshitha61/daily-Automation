#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findTheDistanceValue(vector<int>& arr1, vector<int>& arr2, int d) {
        sort(arr2.begin(), arr2.end());
        int result = 0;
        for (int a : arr1) {
            auto it = lower_bound(arr2.begin(), arr2.end(), a - d);
            bool ok = true;
            if (it != arr2.end() && abs(*it - a) <= d) ok = false;
            if (it != arr2.begin()) {
                --it;
                if (abs(*it - a) <= d) ok = false;
            }
            if (ok) ++result;
        }
        return result;
    }
};