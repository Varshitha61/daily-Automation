#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int minimumKeypresses(string word) {
    vector<int> count(26, 0);
    for (char c : word) {
        count[c - 'a']++;
    }
    sort(count.rbegin(), count.rend());
    int res = 0;
    for (int i = 0; i < 26; i++) {
        if (i < 9) {
            res += count[i];
        } else if (i < 18) {
            res += 2 * count[i];
        } else {
            res += 3 * count[i];
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