#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    if(!(cin >> T)) return 0;
    while (T--) {
        int N;
        cin >> N;
        int degree = -1;
        for (int i = 0; i < N; ++i) {
            int a;
            cin >> a;
            if (a != 0) degree = i;
        }
        // As per problem, at least one non-zero exists, so degree >=0
        cout << degree << "\n";
    }
    return 0;
}