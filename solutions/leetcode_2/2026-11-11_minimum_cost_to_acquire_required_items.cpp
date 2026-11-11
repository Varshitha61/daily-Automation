#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long cost1, cost2, costBoth, need1, need2;
    vector<long long> inputs;
    long long x;
    while (cin >> x) inputs.push_back(x);
    size_t idx = 0;
    bool firstOutput = true;
    while (idx + 4 < inputs.size()) {
        cost1 = inputs[idx++];
        cost2 = inputs[idx++];
        costBoth = inputs[idx++];
        need1 = inputs[idx++];
        need2 = inputs[idx++];
        long long common = min(need1, need2);
        long long diff = llabs(need1 - need2);
        long long costBothOrSeparate = min(costBoth, cost1 + cost2);
        long long extraCostPerUnit = (need1 > need2) ? min(costBoth, cost1) : min(costBoth, cost2);
        long long total = common * costBothOrSeparate + diff * extraCostPerUnit;
        if (!firstOutput) cout << '\n';
        firstOutput = false;
        cout << total;
    }
    return 0;
}