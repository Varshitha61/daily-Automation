#include <bits/stdc++.h>
using namespace std;

bool checkOverlap(int radius, int xCenter, int yCenter,
                  int x1, int y1, int x2, int y2) {
    int closestX = xCenter;
    if (closestX < x1) closestX = x1;
    else if (closestX > x2) closestX = x2;
    int closestY = yCenter;
    if (closestY < y1) closestY = y1;
    else if (closestY > y2) closestY = y2;
    long long dx = (long long)xCenter - closestX;
    long long dy = (long long)yCenter - closestY;
    long long distSq = dx * dx + dy * dy;
    long long rSq = (long long)radius * radius;
    return distSq <= rSq;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<long long> vals;
    long long v;
    while (cin >> v) vals.push_back(v);
    for (size_t i = 0; i + 6 < vals.size(); i += 7) {
        int radius = (int)vals[i];
        int xCenter = (int)vals[i+1];
        int yCenter = (int)vals[i+2];
        int x1 = (int)vals[i+3];
        int y1 = (int)vals[i+4];
        int x2 = (int)vals[i+5];
        int y2 = (int)vals[i+6];
        bool ans = checkOverlap(radius, xCenter, yCenter, x1, y1, x2, y2);
        cout << (ans ? "true" : "false");
        if (i + 7 < vals.size()) cout << '\n';
    }
    return 0;
}