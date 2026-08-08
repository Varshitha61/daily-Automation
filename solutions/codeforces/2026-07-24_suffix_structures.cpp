#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    string s, t;
    cin >> s >> t;

    if (s.size() < t.size()) {
        cout << "need tree" << endl;
        return 0;
    }

    bool canBeDoneWithAutomaton = true;
    int i = 0, j = 0;
    while (i < s.size() && j < t.size()) {
        if (s[i] == t[j]) {
            i++;
            j++;
        } else {
            i++;
            canBeDoneWithAutomaton &= (j == 0);
        }
    }

    if (canBeDoneWithAutomaton) {
        cout << "automaton" << endl;
        return 0;
    }

    sort(s.begin(), s.end());
    sort(t.begin(), t.end());

    if (s == t) {
        cout << "array" << endl;
        return 0;
    }

    cout << "both" << endl;

    return 0;
}