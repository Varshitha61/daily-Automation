#include <iostream>
#include <string>
#include <set>

using namespace std;

int main() {
    string s;
    cin >> s;

    set<char> vowels = {'a', 'e', 'i', 'o', 'u'};

    int n = s.size();
    int i = 0;
    while (i < n) {
        int j = i;
        set<char> consonants;
        while (j < n && vowels.find(s[j]) == vowels.end()) {
            consonants.insert(s[j]);
            j++;
        }
        if (j - i >= 3 && consonants.size() > 1) {
            int k = i + 1;
            while (k < j && vowels.find(s[k]) == vowels.end() && s[k] != s[i]) {
                k++;
            }
            cout << s.substr(i, k - i) << " ";
            i = k;
        } else {
            i = j;
        }
    }
    if (i > 0) {
        cout << s.substr(i);
    }
    cout << endl;

    return 0;
}