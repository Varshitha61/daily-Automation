#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long A, B, C, X;
    if (!(cin >> A >> B >> C >> X)) return 0;
    if (X == A || X == B || X == C) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }
    return 0;
}