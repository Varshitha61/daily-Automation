#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        string s;
        cin >> s;
        reverse(s.begin(), s.end());
        size_t pos = s.find_first_not_of('0');
        if (pos == string::npos) cout << "0";
        else cout << s.substr(pos);
        if (T) cout << '\n';
    }
    return 0;
}