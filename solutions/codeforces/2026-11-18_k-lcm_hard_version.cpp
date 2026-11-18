#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        long long n, k;
        cin >> n >> k;
        vector<long long> ans;
        long long ones = k - 3;
        for (long long i = 0; i < ones; ++i) ans.push_back(1);
        long long m = n - ones; // sum for last three numbers
        if (m % 2 == 1) {
            long long x = (m - 1) / 2;
            ans.push_back(1);
            ans.push_back(x);
            ans.push_back(x);
        } else {
            if (m % 4 == 0) {
                ans.push_back(m / 2);
                ans.push_back(m / 4);
                ans.push_back(m / 4);
            } else {
                ans.push_back(2);
                long long x = (m - 2) / 2;
                ans.push_back(x);
                ans.push_back(x);
            }
        }
        for (size_t i = 0; i < ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }
    return 0;
}