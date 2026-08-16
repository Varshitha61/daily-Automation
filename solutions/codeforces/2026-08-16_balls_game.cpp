#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n, k, x;
vector<int> a;

int destroy(int pos) {
    int cnt = 0;
    vector<int> b;
    for (int i = 0; i < n; i++) {
        if (i == pos) {
            b.push_back(x);
        }
        b.push_back(a[i]);
    }
    vector<int> c;
    int cur = b[0];
    int cur_cnt = 1;
    for (int i = 1; i < b.size(); i++) {
        if (b[i] == cur) {
            cur_cnt++;
        } else {
            if (cur_cnt >= 3) {
                cnt += cur_cnt;
            } else {
                for (int j = 0; j < cur_cnt; j++) {
                    c.push_back(cur);
                }
            }
            cur = b[i];
            cur_cnt = 1;
        }
    }
    if (cur_cnt >= 3) {
        cnt += cur_cnt;
    } else {
        for (int j = 0; j < cur_cnt; j++) {
            c.push_back(cur);
        }
    }
    while (true) {
        bool changed = false;
        vector<int> d;
        cur = c[0];
        cur_cnt = 1;
        for (int i = 1; i < c.size(); i++) {
            if (c[i] == cur) {
                cur_cnt++;
            } else {
                if (cur_cnt >= 3) {
                    cnt += cur_cnt;
                    changed = true;
                } else {
                    for (int j = 0; j < cur_cnt; j++) {
                        d.push_back(cur);
                    }
                }
                cur = c[i];
                cur_cnt = 1;
            }
        }
        if (cur_cnt >= 3) {
            cnt += cur_cnt;
            changed = true;
        } else {
            for (int j = 0; j < cur_cnt; j++) {
                d.push_back(cur);
            }
        }
        c = d;
        if (!changed) {
            break;
        }
    }
    return cnt;
}

int main() {
    cin >> n >> k >> x;
    a.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    int ans = 0;
    for (int i = 0; i <= n; i++) {
        ans = max(ans, destroy(i));
    }
    cout << ans << endl;
    return 0;
}