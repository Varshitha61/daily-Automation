#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string smallestPalindromic(string s) {
    int count[26] = {0};
    for (char c : s) {
        count[c - 'a']++;
    }

    string firstHalf;
    char midChar = '\0';

    for (int i = 0; i < 26; i++) {
        if (count[i] % 2 == 1) {
            if (midChar != '\0') {
                return "";
            }
            midChar = 'a' + i;
        }
        firstHalf += string(count[i] / 2, 'a' + i);
    }

    sort(firstHalf.begin(), firstHalf.end());

    string secondHalf = firstHalf;
    reverse(secondHalf.begin(), secondHalf.end());

    string result = firstHalf + midChar + secondHalf;
    return result;
}

int main() {
    string s;
    s = "z";
    cout << smallestPalindromic(s) << endl;

    s = "babab";
    cout << smallestPalindromic(s) << endl;

    s = "daccad";
    cout << smallestPalindromic(s) << endl;

    return 0;
}