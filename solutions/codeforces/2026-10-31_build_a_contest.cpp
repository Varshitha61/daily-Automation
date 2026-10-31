#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<int> a(m);
    for (int i = 0; i < m; ++i) cin >> a[i];
    int maxCnt = m + 2;
    vector<int> cnt(n + 1, 0);
    vector<int> freq(maxCnt, 0);
    freq[0] = n;
    int rounds = 0;
    int cur = 0; // number of difficulties with cnt[i] > rounds
    string ans;
    ans.reserve(m);
    for (int x : a) {
        int old = cnt[x];
        freq[old]--;
        cnt[x]++;
        int nw = cnt[x];
        freq[nw]++;
        if (nw == rounds + 1) cur++;
        if (cur == n) {
            ans.push_back('1');
            rounds++;
            cur -= freq[rounds];
        } else {
            ans.push_back('0');
        }
    }
    cout << ans << "\n";
    return 0;
}