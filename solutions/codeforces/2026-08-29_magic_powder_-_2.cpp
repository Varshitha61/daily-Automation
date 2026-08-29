#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    long long k;
    cin >> n >> k;

    vector<long long> a(n), b(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) cin >> b[i];

    long long low = 0;
    long long high = 1;

    // Find a reasonable upper bound
    // Maximum cookies is at most (sum of all b + k) / min(a)
    long long sum_b = 0;
    long long min_a = a[0];
    for (int i = 0; i < n; i++) {
        sum_b += b[i];
        min_a = min(min_a, a[i]);
    }
    high = (sum_b + k) / min_a + 1;

    while (low < high) {
        long long mid = low + (high - low + 1) / 2;
        long long needed = 0;
        bool possible = true;
        for (int i = 0; i < n; i++) {
            long long required = a[i] * mid;
            if (required > b[i]) {
                needed += required - b[i];
                if (needed > k) {
                    possible = false;
                    break;
                }
            }
        }
        if (possible) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }

    cout << low << endl;

    return 0;
}