#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    vector<int> queries(T);
    int maxN = 0;
    for(int i = 0; i < T; ++i){
        cin >> queries[i];
        if(queries[i] > maxN) maxN = queries[i];
    }
    int limit = max(2, maxN);
    vector<bool> isPrime(limit + 1, true);
    isPrime[0] = isPrime[1] = false;
    for(int p = 2; p * 1LL * p <= limit; ++p){
        if(isPrime[p]){
            for(long long mult = 1LL * p * p; mult <= limit; mult += p)
                isPrime[(int)mult] = false;
        }
    }
    for(int n : queries){
        cout << (isPrime[n] ? "yes" : "no") << '\n';
    }
    return 0;
}