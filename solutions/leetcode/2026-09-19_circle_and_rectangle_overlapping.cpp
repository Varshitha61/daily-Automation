#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool checkOverlap(int radius, int xCenter, int yCenter, int x1, int y1, int x2, int y2) {
        long long dx = 0, dy = 0;
        if (xCenter < x1) dx = x1 - xCenter;
        else if (xCenter > x2) dx = xCenter - x2;
        if (yCenter < y1) dy = y1 - yCenter;
        else if (yCenter > y2) dy = yCenter - y2;
        long long distSq = dx * dx + dy * dy;
        long long rSq = 1LL * radius * radius;
        return distSq <= rSq;
    }
};