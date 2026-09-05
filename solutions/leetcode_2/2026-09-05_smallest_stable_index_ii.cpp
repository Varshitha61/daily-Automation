#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string line;
    if (!getline(cin, line)) return 0;
    // Remove spaces
    line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
    // Expect format like [5,0,1,4]
    vector<long long> nums;
    size_t l = line.find('[');
    size_t r = line.find(']');
    if (l != string::npos && r != string::npos && r > l) {
        string inside = line.substr(l + 1, r - l - 1);
        stringstream ss(inside);
        string token;
        while (getline(ss, token, ',')) {
            if (!token.empty())
                nums.push_back(stoll(token));
        }
    } else {
        // maybe just numbers separated by spaces
        stringstream ss(line);
        long long x;
        while (ss >> x) nums.push_back(x);
    }
    
    long long k;
    if (!(cin >> k)) {
        // maybe k is on same line after array
        size_t pos = line.find(']');
        if (pos != string::npos) {
            string after = line.substr(pos + 1);
            stringstream ss(after);
            ss >> k;
        } else {
            k = 0;
        }
    }
    
    int n = nums.size();
    if (n == 0) {
        cout << -1;
        return 0;
    }
    vector<long long> pref(n), suff(n);
    pref[0] = nums[0];
    for (int i = 1; i < n; ++i) pref[i] = max(pref[i-1], nums[i]);
    suff[n-1] = nums[n-1];
    for (int i = n-2; i >= 0; --i) suff[i] = min(suff[i+1], nums[i]);
    
    int answer = -1;
    for (int i = 0; i < n; ++i) {
        long long instability = pref[i] - suff[i];
        if (instability <= k) {
            answer = i;
            break;
        }
    }
    cout << answer;
    return 0;
}