#include <iostream>
#include <climits>

using namespace std;

int main() {
    long long n, p;
    cin >> n >> p;

    for (int i = 1; i <= 100; i++) {
        long long x = n - i * p;
        if (x < 0) break;
        int count = 0;
        long long temp = x;
        while (temp > 0) {
            count += temp & 1;
            temp >>= 1;
        }
        if (count <= i) {
            cout << i << endl;
            return 0;
        }
    }
    cout << -1 << endl;
    return 0;
}