#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        int n;
        std::cin >> n;
        std::vector<int> a(n);
        for (int i = 0; i < n; i++) {
            std::cin >> a[i];
        }
        std::sort(a.begin(), a.end());
        int ans = 0;
        for (int i = 0; i < n - 1; i++) {
            ans += a[i + 1] - a[i];
        }
        std::cout << ans << std::endl;
    }
    return 0;
}