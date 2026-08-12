#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    cin >> t;
    while (t--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n), b(m);
        for (int i = 0; i < n; i++) cin >> a[i];
        for (int i = 0; i < m; i++) cin >> b[i];

        sort(a.begin(), a.end());
        sort(b.begin(), b.end());

        map<int, int> mp;
        for (int i = 0; i < n; i++) mp[a[i]]++;
        for (int i = 0; i < m; i++) mp[b[i]]--;

        bool flag = true;
        for (auto &it : mp) {
            if (it.second != 0) {
                flag = false;
                break;
            }
        }

        if (flag) {
            cout << "Yes" << endl;
        } else {
            cout << "No" << endl;
        }
    }

    return 0;
}