#include <iostream>
#include <string>

using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int l, r;
        cin >> l >> r;
        long long ans = 0;
        while (l != 0 || r != 0) {
            ans += r - l;
            l /= 10;
            r /= 10;
        }
        cout << ans << endl;
    }
    return 0;
}