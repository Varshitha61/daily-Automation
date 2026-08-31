#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long N;
    if(!(cin >> N)) return 0;
    if (N % 4 == 0) cout << N + 1;
    else cout << N - 1;
    return 0;
}