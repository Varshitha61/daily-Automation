#include <bits/stdc++.h>
using namespace std;

struct Segment {
    long long value; // max value for this segment
    long long len;
    long long sum;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    if (!getline(cin, line)) return 0;
    // parse array
    vector<long long> nums;
    long long num = 0;
    bool inNumber = false;
    for (char c : line) {
        if (isdigit(c)) {
            num = num * 10 + (c - '0');
            inNumber = true;
        } else {
            if (inNumber) {
                nums.push_back(num);
                num = 0;
                inNumber = false;
            }
        }
    }
    if (inNumber) nums.push_back(num);
    long long k;
    if (!(cin >> k)) return 0;
    int n = nums.size();
    deque<Segment> segs;
    long long needed = 0;
    long long ans = 0;
    int l = 0;
    for (int r = 0; r < n; ++r) {
        long long x = nums[r];
        if (segs.empty() || x > segs.back().value) {
            segs.push_back({x, 1, x});
        } else {
            needed += segs.back().value - x;
            segs.back().len += 1;
            segs.back().sum += x;
        }
        while (needed > k) {
            long long leftVal = nums[l];
            Segment &front = segs.front();
            needed -= front.value - leftVal;
            front.len -= 1;
            front.sum -= leftVal;
            if (front.len == 0) segs.pop_front();
            ++l;
        }
        ans += (r - l + 1);
    }
    cout << ans << "\n";
    return 0;
}