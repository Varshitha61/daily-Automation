#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int toDay(const string& s) {
        static const int monthDays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
        int month = stoi(s.substr(0,2));
        int day = stoi(s.substr(3,2));
        int res = day;
        for (int m = 1; m < month; ++m) res += monthDays[m];
        return res;
    }
    int countDaysTogether(string arriveAlice, string leaveAlice, string arriveBob, string leaveBob) {
        int a1 = toDay(arriveAlice);
        int a2 = toDay(leaveAlice);
        int b1 = toDay(arriveBob);
        int b2 = toDay(leaveBob);
        int start = max(a1, b1);
        int end = min(a2, b2);
        return start > end ? 0 : end - start + 1;
    }
};