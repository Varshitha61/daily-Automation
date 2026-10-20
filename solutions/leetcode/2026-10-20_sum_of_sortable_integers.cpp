#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<int> nums;
    string token;
    while (cin >> token) {
        int cur = 0;
        bool in = false;
        for (char c : token) {
            if (isdigit(c)) {
                cur = cur * 10 + (c - '0');
                in = true;
            } else {
                if (in) {
                    nums.push_back(cur);
                    cur = 0;
                    in = false;
                }
            }
        }
        if (in) nums.push_back(cur);
    }
    if (nums.empty()) return 0;
    int n = nums.size();
    vector<int> sorted = nums;
    sort(sorted.begin(), sorted.end());

    vector<int> divisors;
    for (int i = 1; i * 1LL * i <= n; ++i) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i != n / i) divisors.push_back(n / i);
        }
    }

    long long answer = 0;
    for (int k : divisors) {
        bool ok = true;
        for (int start = 0; start < n; start += k) {
            // pattern = nums block
            vector<int> pat(k);
            for (int t = 0; t < k; ++t) pat[t] = nums[start + t];
            // text = target block duplicated
            vector<int> txt(2 * k);
            for (int t = 0; t < k; ++t) txt[t] = sorted[start + t];
            for (int t = 0; t < k; ++t) txt[k + t] = sorted[start + t];

            // build LPS for pattern
            vector<int> lps(k, 0);
            for (int i = 1, len = 0; i < k; ) {
                if (pat[i] == pat[len]) {
                    lps[i++] = ++len;
                } else if (len) {
                    len = lps[len - 1];
                } else {
                    lps[i++] = 0;
                }
            }

            // KMP search, but only need to check within first k positions of start offset
            int i = 0, j = 0;
            bool found = false;
            while (i < 2 * k) {
                if (pat[j] == txt[i]) {
                    ++i; ++j;
                    if (j == k) { found = true; break; }
                } else {
                    if (j) j = lps[j - 1];
                    else ++i;
                }
                if (i - j > k) break; // offset exceeds block length
            }
            if (!found) { ok = false; break; }
        }
        if (ok) answer += k;
    }
    cout << answer;
    return 0;
}