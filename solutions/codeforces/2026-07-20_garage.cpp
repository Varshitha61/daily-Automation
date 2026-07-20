#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int main() {
    long long n;
    cin >> n;

    long long i = 1;
    long long cnt = 0;
    while (true) {
        long long x = i * i;
        for (long long j = 1; j * j < x; j++) {
            if ((x - j * j) % j == 0 && (x - j * j) / j > j) {
                cnt++;
                if (cnt == n) {
                    cout << x << endl;
                    return 0;
                }
            }
        }
        i++;
    }

    return 0;
}