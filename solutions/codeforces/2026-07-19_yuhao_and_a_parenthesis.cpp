#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;

    map<int, vector<string>> m;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        int cnt = 0;
        for (char c : s) {
            if (c == '(') cnt++;
            else cnt--;
        }
        m[cnt].push_back(s);
    }

    int ans = 0;
    for (auto& p : m) {
        if (p.first == 0) {
            ans += p.second.size() / 2;
        }
    }

    vector<int> keys;
    for (auto& p : m) {
        if (p.first != 0) {
            keys.push_back(p.first);
        }
    }
    sort(keys.begin(), keys.end());

    for (int i = 0; i < keys.size(); i++) {
        int key = keys[i];
        if (m.find(-key) != m.end()) {
            int min_size = min(m[key].size(), m[-key].size());
            ans += min_size;
            m[key].resize(m[key].size() - min_size);
            m[-key].resize(m[-key].size() - min_size);
        }
    }

    cout << ans << endl;

    return 0;
}