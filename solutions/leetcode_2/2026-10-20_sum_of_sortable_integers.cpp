#include <bits/stdc++.h>
using namespace std;

bool isRotation(const vector<int>& A, const vector<int>& B) {
    int k = A.size();
    if (k == 0) return true;
    // KMP prefix for pattern B
    vector<int> pi(k, 0);
    for (int i = 1; i < k; ++i) {
        int j = pi[i - 1];
        while (j > 0 && B[i] != B[j]) j = pi[j - 1];
        if (B[i] == B[j]) ++j;
        pi[i] = j;
    }
    int j = 0;
    for (int i = 0; i < 2 * k - 1; ++i) {
        int cur = A[i % k];
        while (j > 0 && cur != B[j]) j = pi[j - 1];
        if (cur == B[j]) ++j;
        if (j == k) return true;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string input, line;
    while (getline(cin, line)) {
        input += line;
        input += ' ';
    }
    vector<int> nums;
    long long num = 0;
    bool inNum = false;
    for (char c : input) {
        if (isdigit(c)) {
            num = num * 10 + (c - '0');
            inNum = true;
        } else {
            if (inNum) {
                nums.push_back((int)num);
                num = 0;
                inNum = false;
            }
        }
    }
    if (inNum) nums.push_back((int)num);
    int n = nums.size();
    if (n == 0) {
        cout << 0 << "\n";
        return 0;
    }
    vector<int> sorted = nums;
    sort(sorted.begin(), sorted.end());

    vector<int> divisors;
    for (int i = 1; i * (long long)i <= n; ++i) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i != n / i) divisors.push_back(n / i);
        }
    }

    long long answer = 0;
    for (int k : divisors) {
        bool ok = true;
        for (int start = 0; start < n; start += k) {
            vector<int> A(nums.begin() + start, nums.begin() + start + k);
            vector<int> B(sorted.begin() + start, sorted.begin() + start + k);
            if (!isRotation(A, B)) {
                ok = false;
                break;
            }
        }
        if (ok) answer += k;
    }
    cout << answer << "\n";
    return 0;
}