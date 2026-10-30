#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long a, b;
    if(!(cin >> a)) return 0;
    if(!(cin >> b)) {
        // If only one number provided, treat second as 0
        b = 0;
    }
    cout << (a + b);
    return 0;
}