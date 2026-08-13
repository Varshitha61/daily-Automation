#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

const int MAX_N = 15;

int N;
vector<vector<pair<int, int>>> A;

bool check(const bitset<MAX_N>& honest) {
    for (int i = 0; i < N; i++) {
        if (honest[i]) {
            for (const auto& p : A[i]) {
                if (p.second == 1 && !honest[p.first - 1]) {
                    return false;
                }
                if (p.second == 0 && honest[p.first - 1]) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main() {
    cin >> N;
    A.resize(N);
    for (int i = 0; i < N; i++) {
        int a;
        cin >> a;
        A[i].resize(a);
        for (int j = 0; j < a; j++) {
            cin >> A[i][j].first >> A[i][j].second;
        }
    }

    int ans = 0;
    for (int i = 0; i < (1 << N); i++) {
        bitset<MAX_N> honest(i);
        if (check(honest)) {
            ans = max(ans, (int)honest.count());
        }
    }

    cout << ans << endl;
    return 0;
}