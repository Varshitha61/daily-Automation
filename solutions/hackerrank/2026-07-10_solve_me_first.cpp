#include <iostream>

int solveMeFirst(int a, int b) {
    return a + b;
}

int main() {
    int a, b;
    std::cin >> a >> b;
    std::cout << solveMeFirst(a, b);
    return 0;
}