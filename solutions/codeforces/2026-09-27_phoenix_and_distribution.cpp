#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int n, k;
        string s;
        cin >> n >> k >> s;
        sort(s.begin(), s.end());
        if (s[0] != s[k-1]) {
            cout << s[k-1] << "\n";
        } else {
            string ans;
            ans.push_back(s[0]);
            if (k == n) {
                // nothing more
            } else if (s[k] != s[n-1]) {
                ans += s.substr(k);
            } else {
                int remaining = n - k;
                int times = (remaining + k - 1) / k;
                ans.append(times, s[k]);
            }
            cout << ans << "\n";
        }
    }
    return 0;
}