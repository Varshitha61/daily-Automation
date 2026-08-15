#include <iostream>
#include <string>

int main() {
    std::string s;
    std::cin >> s;

    int max_length = 0;
    for (int i = 0; i < s.size(); i++) {
        for (int j = i + 1; j <= s.size(); j++) {
            std::string substr = s.substr(i, j - i);
            bool is_acgt = true;
            for (char c : substr) {
                if (c != 'A' && c != 'C' && c != 'G' && c != 'T') {
                    is_acgt = false;
                    break;
                }
            }
            if (is_acgt) {
                max_length = std::max(max_length, static_cast<int>(substr.size()));
            }
        }
    }

    std::cout << max_length << std::endl;

    return 0;
}