#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    int ans = 2;
    int max_count = 0;
    for (int i = 2; i <= 1000; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (a[j] % i == 0) {
                count++;
            }
        }
        if (count > max_count) {
            max_count = count;
            ans = i;
        }
    }
    cout << ans << endl;
    return 0;
}