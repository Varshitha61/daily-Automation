#include <bits/stdc++.h>
using namespace std;

bool isLess(const string& a, const string& b) {
    int i = 0, j = 0;
    while (i < (int)a.size() && a[i] == '0') ++i;
    while (j < (int)b.size() && b[j] == '0') ++j;
    int lenA = (int)a.size() - i;
    int lenB = (int)b.size() - j;
    if (lenA != lenB) return lenA < lenB;
    for (int p = 0; p < lenA; ++p) {
        if (a[i + p] != b[j + p]) return a[i + p] < b[j + p];
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    string s;
    cin >> s;
    string best;
    bool first = true;
    for (int k = 0; k < 10; ++k) {
        string t = s;
        for (char &c : t) c = char('0' + ((c - '0' + k) % 10));
        string tt = t + t;
        for (int r = 0; r < n; ++r) {
            int start = (n - r) % n;
            string u = tt.substr(start, n);
            if (first || isLess(u, best)) {
                best = u;
                first = false;
            }
        }
    }
    cout << best << "\n";
    return 0;
}