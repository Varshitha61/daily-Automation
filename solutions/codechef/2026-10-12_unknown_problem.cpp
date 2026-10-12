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
        long long cntStart = 0, cntLtime = 0;
        for (int i = 0; i < N; ++i) {
            string s;
            cin >> s;
            if (s == "START38") ++cntStart;
            else if (s == "LTIME108") ++cntLtime;
        }
        cout << cntStart << ' ' << cntLtime;
        if (T) cout << '\n';
    }
    return 0;
}