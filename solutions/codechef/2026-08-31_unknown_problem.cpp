#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long N;
        cin >> N;
        long long r = sqrt((long double)N);
        while ((r+1)*(r+1) <= N) ++r;
        while (r*r > N) --r;
        cout << r << "\n";
    }
    return 0;
}