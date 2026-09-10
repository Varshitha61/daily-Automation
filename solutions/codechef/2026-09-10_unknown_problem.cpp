#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    const int MAXN = 100;
    const int BASE = 10000;
    const int WIDTH = 4;
    
    vector<string> fact(MAXN + 1);
    vector<int> a;
    a.push_back(1); // 0! = 1
    fact[0] = "1";
    for (int i = 1; i <= MAXN; ++i) {
        long long carry = 0;
        for (size_t j = 0; j < a.size(); ++j) {
            long long prod = 1LL * a[j] * i + carry;
            a[j] = int(prod % BASE);
            carry = prod / BASE;
        }
        while (carry) {
            a.push_back(int(carry % BASE));
            carry /= BASE;
        }
        // convert to string
        string s;
        for (int k = (int)a.size() - 1; k >= 0; --k) {
            string part = to_string(a[k]);
            if (k != (int)a.size() - 1) {
                s += string(WIDTH - part.length(), '0') + part;
            } else {
                s += part;
            }
        }
        fact[i] = s;
    }
    
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n;
        cin >> n;
        cout << fact[n] << '\n';
    }
    return 0;
}