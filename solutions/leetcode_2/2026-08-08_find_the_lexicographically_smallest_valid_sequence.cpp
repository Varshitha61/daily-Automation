#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<int> findSmallestSequence(string word1, string word2) {
    int m = word1.size(), n = word2.size();
    vector<int> res;
    int i = 0, j = 0, diff = 0;

    while (i < m && j < n) {
        if (word1[i] == word2[j]) {
            res.push_back(i);
            i++;
            j++;
        } else {
            diff++;
            if (diff > 1) return {};
            while (i < m && word1[i] != word2[j]) i++;
            if (i == m) return {};
            res.push_back(i);
            i++;
            j++;
        }
    }

    if (j < n) return {};

    return res;
}

int main() {
    string word1, word2;
    word1 = "vbcca";
    word2 = "abc";
    vector<int> res = findSmallestSequence(word1, word2);
    for (int i : res) cout << i << " ";
    cout << endl;

    word1 = "bacdc";
    word2 = "abc";
    res = findSmallestSequence(word1, word2);
    for (int i : res) cout << i << " ";
    cout << endl;

    word1 = "aaaaaa";
    word2 = "aaabc";
    res = findSmallestSequence(word1, word2);
    for (int i : res) cout << i << " ";
    cout << endl;

    word1 = "abc";
    word2 = "ab";
    res = findSmallestSequence(word1, word2);
    for (int i : res) cout << i << " ";
    cout << endl;

    return 0;
}