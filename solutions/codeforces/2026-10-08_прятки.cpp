#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> first(n + 1, k + 1);
    vector<int> last(n + 1, 0);
    for (int i = 1; i <= k; ++i) {
        int x; cin >> x;
        if (first[x] == k + 1) first[x] = i;
        last[x] = i;
    }
    long long ans = 0;
    for (int p = 1; p <= n; ++p) {
        if (first[p] == k + 1) ++ans; // a = b, never appears
    }
    for (int a = 1; a <= n; ++a) {
        if (a + 1 <= n && last[a + 1] < first[a]) ++ans;
        if (a - 1 >= 1 && last[a - 1] < first[a]) ++ans;
    }
    cout << ans << "\n";
    return 0;
}