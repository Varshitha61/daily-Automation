#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;
    string s;
    cin >> s;

    int ans = 0;
    while (true) {
        bool changed = false;
        for (int i = 0; i < n; i++) {
            if (i > 0 && s[i - 1] == s[i] - 1) {
                s.erase(i, 1);
                n--;
                ans++;
                changed = true;
                break;
            }
            if (i < n - 1 && s[i + 1] == s[i] - 1) {
                s.erase(i, 1);
                n--;
                ans++;
                changed = true;
                break;
            }
        }
        if (!changed) break;
    }

    cout << ans << endl;

    return 0;
}