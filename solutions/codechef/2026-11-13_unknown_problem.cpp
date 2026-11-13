#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    if(!(cin >> N)) return 0;
    long long cum1 = 0, cum2 = 0;
    int winner = 0;
    long long maxLead = 0;
    for (int i = 0; i < N; ++i) {
        long long s, t;
        cin >> s >> t;
        cum1 += s;
        cum2 += t;
        long long lead = llabs(cum1 - cum2);
        int curWinner = (cum1 >= cum2) ? 1 : 2;
        if (lead > maxLead) {
            maxLead = lead;
            winner = curWinner;
        }
    }
    cout << winner << ' ' << maxLead << '\n';
    return 0;
}