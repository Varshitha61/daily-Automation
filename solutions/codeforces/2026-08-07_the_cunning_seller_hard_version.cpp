#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

typedef long long ll;

ll calculateCost(ll x) {
    return 3 * x + 1 + x * (3 * x - 1);
}

ll calculateWatermelons(ll x) {
    return 3 * x;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        ll n, k;
        cin >> n >> k;

        ll minCost = -1;

        for (ll i = 0; i <= k; i++) {
            ll watermelons = calculateWatermelons(i);
            if (watermelons > n) break;

            ll remainingWatermelons = n - watermelons;
            if (remainingWatermelons % 3 == 0 && remainingWatermelons / 3 <= k - i) {
                ll cost = calculateCost(i) + 3 * remainingWatermelons;
                if (minCost == -1 || cost < minCost) minCost = cost;
            }
        }

        cout << minCost << '\n';
    }

    return 0;
}