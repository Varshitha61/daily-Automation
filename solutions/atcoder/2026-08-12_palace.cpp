#include <iostream>
#include <cmath>

int main() {
    int N;
    double T, A;
    std::cin >> N >> T >> A;

    double min_diff = std::abs(T - A);
    int ans = 1;

    for (int i = 1; i <= N; i++) {
        double H;
        std::cin >> H;
        double temp = T - H * 0.006;
        double diff = std::abs(temp - A);

        if (diff < min_diff) {
            min_diff = diff;
            ans = i;
        }
    }

    std::cout << ans << std::endl;

    return 0;
}