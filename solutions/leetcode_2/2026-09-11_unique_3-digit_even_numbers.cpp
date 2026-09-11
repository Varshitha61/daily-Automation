#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string input, line;
    while (getline(cin, line)) {
        input += line;
        input += ' ';
    }
    vector<int> digits;
    for (size_t i = 0; i < input.size(); ++i) {
        if (isdigit(input[i])) {
            int val = input[i] - '0';
            digits.push_back(val);
        }
    }
    int n = digits.size();
    unordered_set<int> uniq;
    uniq.reserve(1000);
    for (int i = 0; i < n; ++i) {
        if (digits[i] == 0) continue; // leading zero not allowed
        for (int j = 0; j < n; ++j) {
            if (j == i) continue;
            for (int k = 0; k < n; ++k) {
                if (k == i || k == j) continue;
                if (digits[k] % 2 != 0) continue; // must be even
                int num = digits[i] * 100 + digits[j] * 10 + digits[k];
                uniq.insert(num);
            }
        }
    }
    cout << uniq.size();
    return 0;
}