#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long A, B;
        cin >> A >> B;
        if (A < B) cout << "<";
        else if (A > B) cout << ">";
        else cout << "=";
        if (T) cout << '\n';
    }
    return 0;
}