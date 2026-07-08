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

using namespace std;

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<pii> vpii;
typedef vector<pll> vpll;

#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)(x).size())

const int mod = 1e9 + 7;
const int inf = 1e9 + 7;
const ld eps = 1e-9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vi a(n);
        for (int i = 0; i < n; i++) {
            cin >> a[i];
        }
        sort(all(a));
        int ans = 0;
        for (int i = 0; i < n; i++) {
            if (a[i] >= 0) {
                ans = i;
                break;
            }
        }
        if (ans == 0) {
            cout << a[n - 1] << endl;
        } else if (ans == n) {
            cout << a[0] << endl;
        } else {
            int sum1 = 0, sum2 = 0;
            for (int i = 0; i < ans; i++) {
                sum1 += a[i];
            }
            for (int i = ans; i < n; i++) {
                sum2 += a[i];
            }
            if (abs(sum1) < sum2) {
                cout << a[ans - 1] << endl;
            } else {
                cout << a[ans] << endl;
            }
        }
    }

    return 0;
}