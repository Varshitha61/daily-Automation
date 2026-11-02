#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long x;
    if(!(cin>>x)) return 0;
    if (x % 2 == 0 && x % 4 != 0) cout << "yes\n";
    else cout << "no\n";
    return 0;
}