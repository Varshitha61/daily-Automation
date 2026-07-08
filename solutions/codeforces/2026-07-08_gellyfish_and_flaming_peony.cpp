#include <bits/stdc++.h>
using namespace std;

int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<int> a(n);
        for (int i = 0; i < n; i++)
            cin >> a[i];
        int g = a[0];
        for (int i = 1; i < n; i++)
            g = gcd(g, a[i]);
        int ans = 0;
        for (int i = 0; i < n; i++)
            if (a[i] != g)
                ans++;
        if (ans == n)
            ans--;
        cout << ans << endl;
    }
    return 0;
}