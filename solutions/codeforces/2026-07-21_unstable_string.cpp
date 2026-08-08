#include <iostream>
#include <string>

using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        string s;
        cin >> s;
        long long ans = 0;
        for (int i = 0; i < s.size(); i++) {
            int a = -1, b = -1;
            for (int j = i; j < s.size(); j++) {
                if (s[j] != '?') {
                    if ((j - i) % 2 == 0) {
                        if (s[j] - '0' != a && a != -1) break;
                        a = s[j] - '0';
                    } else {
                        if (s[j] - '0' != b && b != -1) break;
                        b = s[j] - '0';
                    }
                }
                if (a == -1 && b == -1) {
                    if (j % 2 == i % 2) a = 0;
                    else b = 0;
                } else if (a == -1) {
                    a = b ^ 1;
                } else if (b == -1) {
                    b = a ^ 1;
                }
                ans++;
            }
        }
        cout << ans << endl;
    }
    return 0;
}