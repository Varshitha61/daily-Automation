#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;

    std::vector<std::pair<int, int>> a(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i].first >> a[i].second;
    }

    int ans = 0;
    int diff = 0;
    for (int i = 0; i < n; i++) {
        if (a[i].first > a[i].second) {
            ans++;
            diff += a[i].first - a[i].second;
        }
    }

    std::cout << ans << " " << diff << std::endl;

    return 0;
}