#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long R, O, C;
    if (!(cin >> R >> O >> C)) return 0;
    long long maxScore = C + (20 - O) * 36;
    cout << (maxScore > R ? "YES" : "NO");
    return 0;
}