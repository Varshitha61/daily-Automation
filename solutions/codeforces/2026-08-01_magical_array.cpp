#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;

    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    long long ans = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            std::vector<int> b(a.begin() + i, a.begin() + j + 1);
            if (*std::min_element(b.begin(), b.end()) == *std::max_element(b.begin(), b.end())) {
                ++ans;
            }
        }
    }

    std::cout << ans << std::endl;

    return 0;
}