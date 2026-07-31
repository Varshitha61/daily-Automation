#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

int minimumKeypresses(string word) {
    map<char, int> freq;
    for (char c : word) {
        freq[c]++;
    }
    vector<int> counts;
    for (auto& pair : freq) {
        counts.push_back(pair.second);
    }
    sort(counts.rbegin(), counts.rend());
    int res = 0;
    for (int i = 0; i < counts.size(); i++) {
        if (i < 9) {
            res += counts[i];
        } else if (i < 18) {
            res += 2 * counts[i];
        } else {
            res += 3 * counts[i];
        }
    }
    return res;
}

int main() {
    string word;
    cin >> word;
    cout << minimumKeypresses(word) << endl;
    return 0;
}