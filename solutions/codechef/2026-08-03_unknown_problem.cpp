#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<pair<int, int>> points(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i].first >> points[i].second;
    }

    int maxDistance = 0;
    int maxIndex = 0;
    for (int i = 0; i < n; i++) {
        int distance = abs(points[i].first - points[(i + 1) % n].first) + abs(points[i].second - points[(i + 1) % n].second);
        if (distance > maxDistance) {
            maxDistance = distance;
            maxIndex = i;
        }
    }

    cout << maxIndex + 1 << " " << maxDistance << endl;

    return 0;
}