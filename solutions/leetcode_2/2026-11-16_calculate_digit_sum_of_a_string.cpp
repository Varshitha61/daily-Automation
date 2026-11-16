#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    if(!(cin >> s)) return 0;
    int k;
    if(!(cin >> k)) return 0;
    while ((int)s.size() > k) {
        string nxt;
        for (int i = 0; i < (int)s.size(); i += k) {
            int sum = 0;
            for (int j = i; j < i + k && j < (int)s.size(); ++j) {
                sum += s[j] - '0';
            }
            nxt += to_string(sum);
        }
        s.swap(nxt);
    }
    cout << s;
    return 0;
}