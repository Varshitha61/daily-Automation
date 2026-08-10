#include <iostream>
#include <vector>
#include <algorithm>

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    int ans = 2;
    int max_count = 0;
    for (int i = 2; i <= 1000; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (a[j] % i == 0) count++;
        }
        if (count > max_count) {
            max_count = count;
            ans = i;
        }
    }

    std::cout << ans << std::endl;
    return 0;
}