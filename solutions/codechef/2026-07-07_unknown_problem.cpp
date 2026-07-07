#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <bitset>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <chrono>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<vi> vvi;
typedef vector<vll> vvll;
typedef vector<pii> vpii;
typedef vector<pll> vpll;

const int mod = 1e9 + 7;
const int inf = 1e9 + 7;
const ld eps = 1e-9;
const ld pi = 3.1415926535897932384626433832795;

void solve() {
    int n;
    cin >> n;
    vll a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    ll ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                ans = max(ans, a[i] + a[j] + a[k]);
            }
        }
    }
    cout << ans << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int t = 1;
    while (t--) {
        solve();
    }
    return 0;
}