#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using boost::multiprecision::cpp_int;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if(!(cin >> t)) return 0;
    vector<int> queries(t);
    int maxn = 0;
    for(int i = 0; i < t; ++i){
        cin >> queries[i];
        maxn = max(maxn, queries[i]);
    }
    vector<cpp_int> fact(maxn + 1);
    fact[0] = 1;
    for(int i = 1; i <= maxn; ++i){
        fact[i] = fact[i-1] * i;
    }
    for(int n : queries){
        cout << fact[n] << '\n';
    }
    return 0;
}