#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int MOD = 1e9 + 7;

int getSum(int x) {
    int sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

int getConcatenatedNumber(const string& s, int left, int right) {
    int concatenatedNumber = 0;
    for (int i = left; i <= right; i++) {
        if (s[i] != '0') {
            concatenatedNumber = concatenatedNumber * 10 + (s[i] - '0');
        }
    }
    return concatenatedNumber;
}

vector<int> processQueries(const string& s, const vector<vector<int>>& queries) {
    vector<int> answers;
    for (const auto& query : queries) {
        int left = query[0];
        int right = query[1];
        int concatenatedNumber = getConcatenatedNumber(s, left, right);
        int sum = getSum(concatenatedNumber);
        long long answer = (long long)concatenatedNumber * sum % MOD;
        answers.push_back(answer);
    }
    return answers;
}

int main() {
    string s;
    int q;
    cin >> s >> q;
    vector<vector<int>> queries(q, vector<int>(2));
    for (int i = 0; i < q; i++) {
        cin >> queries[i][0] >> queries[i][1];
    }
    vector<int> answers = processQueries(s, queries);
    for (int answer : answers) {
        cout << answer << " ";
    }
    return 0;
}