#include <iostream>
#include <vector>

int main() {
    std::vector<int> a;
    int x;
    while (std::cin >> x) {
        a.push_back(x);
    }
    for (int i = a.size() - 1; i >= 0; --i) {
        std::cout << a[i] << std::endl;
    }
    return 0;
}