#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int N;
        cin >> N;
        int cnt = 0;
        for (int i = 0; i < N; ++i) {
            int d;
            cin >> d;
            if (d >= 1000) ++cnt;
        }
        cout << cnt;
        if (T) cout << '\n';
    }
    return 0;
}