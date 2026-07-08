#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

const int MOD = 1e9 + 7;

int sumOfDigits(int x) {
    int sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

int concatNonZeroDigitsAndMultiplyBySum(const string& s, const vector<vector<int>>& queries) {
    int result = 0;
    for (const auto& query : queries) {
        int left = query[0];
        int right = query[1];
        string substring = s.substr(left, right - left + 1);
        string nonZeroDigits;
        for (char c : substring) {
            if (c != '0') {
                nonZeroDigits += c;
            }
        }
        if (nonZeroDigits.empty()) {
            result = 0;
        } else {
            int x = stoi(nonZeroDigits);
            int sum = sumOfDigits(x);
            result = (static_cast<long long>(x) * sum) % MOD;
        }
    }
    return result;
}

int main() {
    string s;
    int q;
    cin >> s >> q;
    vector<vector<int>> queries(q, vector<int>(2));
    for (int i = 0; i < q; ++i) {
        cin >> queries[i][0] >> queries[i][1];
    }
    vector<int> answers;
    for (const auto& query : queries) {
        int left = query[0];
        int right = query[1];
        string substring = s.substr(left, right - left + 1);
        string nonZeroDigits;
        for (char c : substring) {
            if (c != '0') {
                nonZeroDigits += c;
            }
        }
        if (nonZeroDigits.empty()) {
            answers.push_back(0);
        } else {
            int x = stoi(nonZeroDigits);
            int sum = sumOfDigits(x);
            answers.push_back((static_cast<long long>(x) * sum) % MOD);
        }
    }
    for (int answer : answers) {
        cout << answer << " ";
    }
    return 0;
}