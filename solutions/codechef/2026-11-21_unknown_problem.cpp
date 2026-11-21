#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long N;
        cin >> N;
        if (N % 4 == 0) cout << "Good";
        else cout << "Not Good";
        if (T) cout << '\n';
    }
    return 0;
}