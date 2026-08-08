#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;

    std::vector<int> m(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> m[i];
    }

    int sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += m[i];
    }

    int avg = sum / n;
    int rem = sum % n;

    int ans = 0;
    for (int i = 0; i < n; ++i) {
        if (m[i] > avg + (i < rem ? 1 : 0)) {
            ans += m[i] - (avg + (i < rem ? 1 : 0));
        }
    }

    std::cout << ans << std::endl;

    return 0;
}