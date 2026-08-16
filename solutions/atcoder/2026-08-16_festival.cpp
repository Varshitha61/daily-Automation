#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> a(m);
    for (int i = 0; i < m; i++) {
        std::cin >> a[i];
    }

    for (int i = 1; i <= n; i++) {
        auto it = std::lower_bound(a.begin(), a.end(), i);
        if (it == a.end()) {
            std::cerr << "Error: no fireworks after day " << i << std::endl;
            return 1;
        }
        std::cout << *it - i << std::endl;
    }

    return 0;
}