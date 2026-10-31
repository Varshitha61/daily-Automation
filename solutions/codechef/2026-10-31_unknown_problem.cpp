#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int N, X;
        cin >> N >> X;
        int cnt = 0;
        for (int i = 0; i < N; ++i) {
            int a;
            cin >> a;
            if (a >= X) ++cnt;
        }
        cout << cnt;
        if (T) cout << '\n';
    }
    return 0;
}