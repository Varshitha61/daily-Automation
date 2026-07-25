#include <iostream>
#include <climits>

using namespace std;

int main() {
    long long n, p;
    cin >> n >> p;

    for (int i = 1; i <= 100; i++) {
        long long x = n - i * p;
        if (x < 0) break;
        int cnt = 0;
        long long t = x;
        while (t > 0) {
            cnt += t % 2;
            t /= 2;
        }
        if (cnt <= i) {
            cout << i << endl;
            return 0;
        }
    }
    cout << -1 << endl;
    return 0;
}