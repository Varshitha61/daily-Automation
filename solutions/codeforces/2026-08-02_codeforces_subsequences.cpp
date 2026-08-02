#include <iostream>
#include <string>

int main() {
    int k;
    std::cin >> k;
    std::string s = "codeforces";
    int cnt = 1;
    while (cnt < k) {
        s += 's';
        cnt *= 2;
    }
    std::cout << s << std::endl;
    return 0;
}