#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<pair<int, int>> scores(n);
    for (int i = 0; i < n; i++) {
        cin >> scores[i].first >> scores[i].second;
    }

    int ans = 0;
    for (int i = 0; i < n; i++) {
        int min_score = min(scores[i].first, scores[i].second);
        ans = max(ans, min_score + 1);
    }

    cout << ans << endl;
    return 0;
}