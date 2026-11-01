#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    const unordered_set<int> singlePos = {3,7,11,13};
    while (T--) {
        int N;
        cin >> N;
        string deck = (N <= 15) ? "Lower" : "Upper";
        int r = (N - 1) % 15 + 1;
        string type = (singlePos.count(r) ? "Single" : "Double");
        cout << deck << ' ' << type;
        if (T) cout << '\n';
    }
    return 0;
}