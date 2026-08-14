#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k, m;
    cin >> n >> k >> m;

    vector<string> words(n);
    for (int i = 0; i < n; i++) {
        cin >> words[i];
    }

    vector<int> costs(n);
    for (int i = 0; i < n; i++) {
        cin >> costs[i];
    }

    vector<vector<int>> groups(k);
    for (int i = 0; i < k; i++) {
        int x;
        cin >> x;
        groups[i].resize(x);
        for (int j = 0; j < x; j++) {
            cin >> groups[i][j];
            groups[i][j]--;
        }
    }

    vector<string> message(m);
    for (int i = 0; i < m; i++) {
        cin >> message[i];
    }

    vector<int> min_costs(k, INT_MAX);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < groups[i].size(); j++) {
            min_costs[i] = min(min_costs[i], costs[groups[i][j]]);
        }
    }

    vector<int> group_ids(n);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < groups[i].size(); j++) {
            group_ids[groups[i][j]] = i;
        }
    }

    long long total_cost = 0;
    for (int i = 0; i < m; i++) {
        int word_id = -1;
        for (int j = 0; j < n; j++) {
            if (words[j] == message[i]) {
                word_id = j;
                break;
            }
        }
        total_cost += min_costs[group_ids[word_id]];
    }

    cout << total_cost << endl;

    return 0;
}