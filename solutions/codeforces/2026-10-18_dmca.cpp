#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long a;
    if(!(cin >> a)) return 0;
    cout << ( (a - 1) % 9 + 1 ) << "\n";
    return 0;
}