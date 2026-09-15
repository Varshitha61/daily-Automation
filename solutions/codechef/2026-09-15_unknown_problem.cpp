#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int A, B, C;
        cin >> A >> B >> C;
        int low = max(A, C);
        if (low <= B) cout << "Yes\n";
        else cout << "No\n";
    }
    return 0;
}