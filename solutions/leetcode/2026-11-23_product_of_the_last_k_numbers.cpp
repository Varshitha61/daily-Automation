#include <bits/stdc++.h>
using namespace std;

class ProductOfNumbers {
    vector<long long> pref;
public:
    ProductOfNumbers() {
        pref.push_back(1);
    }
    
    void add(int num) {
        if (num == 0) {
            pref.clear();
            pref.push_back(1);
        } else {
            pref.push_back(pref.back() * num);
        }
    }
    
    int getProduct(int k) {
        if (k >= (int)pref.size()) return 0;
        long long res = pref.back() / pref[pref.size() - 1 - k];
        return (int)res;
    }
};