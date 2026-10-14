#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int X;
        cin >> X;
        int leftDist = X - 1;
        int rightDist = 100 - X;
        if (leftDist <= rightDist) cout << "LEFT";
        else cout << "RIGHT";
        if (T) cout << '\n';
    }
    return 0;
}