#include <iostream>
#include <vector>
#include <string>

using namespace std;

int maximizeActiveSections(string s) {
    int n = s.size();
    vector<int> ones, zeros;
    int count = 0, maxCount = 0;

    for (int i = 0; i < n; i++) {
        if (s[i] == '1') {
            count++;
        } else {
            if (count > 0) {
                ones.push_back(count);
                count = 0;
            }
        }
    }

    if (count > 0) {
        ones.push_back(count);
    }

    count = 0;

    for (int i = 0; i < n; i++) {
        if (s[i] == '0') {
            count++;
        } else {
            if (count > 0) {
                zeros.push_back(count);
                count = 0;
            }
        }
    }

    if (count > 0) {
        zeros.push_back(count);
    }

    maxCount = ones.size();

    for (int i = 0; i < ones.size(); i++) {
        for (int j = 0; j < zeros.size(); j++) {
            int temp = ones.size() - 1 + zeros.size() - 1;
            temp += zeros[j];
            temp -= ones[i];
            maxCount = max(maxCount, temp);
        }
    }

    return maxCount;
}

int main() {
    string s;
    cin >> s;
    cout << maximizeActiveSections(s) << endl;
    return 0;
}