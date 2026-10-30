#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int N;
        long long K;
        cin >> N >> K;
        long long cnt = 0;
        for (int i = 0; i < N; ++i) {
            long long x;
            cin >> x;
            if ((x + K) % 7 == 0) ++cnt;
        }
        cout << cnt << '\n';
    }
    return 0;
}