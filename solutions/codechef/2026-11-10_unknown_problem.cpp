#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long P, Q;
        cin >> P >> Q;
        long long n = P + Q + 1;               // next serve number
        long long block = (n - 1) / 2;          // which 2-serve block
        if (block % 2 == 0)
            cout << "Alice\n";
        else
            cout << "Bob\n";
    }
    return 0;
}