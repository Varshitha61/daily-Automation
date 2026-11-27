#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int N;
        string S;
        cin >> N >> S;
        string res;
        res.reserve(S.size());
        for (char c : S) {
            switch (c) {
                case 'A': res.push_back('T'); break;
                case 'T': res.push_back('A'); break;
                case 'C': res.push_back('G'); break;
                case 'G': res.push_back('C'); break;
                default: res.push_back(c); // should not happen
            }
        }
        cout << res;
        if (T) cout << '\n';
    }
    return 0;
}