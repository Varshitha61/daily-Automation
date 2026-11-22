#include <bits/stdc++.h>
using namespace std;

string normalize(const string& s) {
    string res;
    res.reserve(s.size());
    for (char c : s) {
        if (c == '-' || c == ';' || c == '_') continue;
        if ('a' <= c && c <= 'z') res.push_back(c - 'a' + 'A');
        else res.push_back(c);
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s1, s2, s3;
    if (!getline(cin, s1)) return 0;
    getline(cin, s2);
    getline(cin, s3);
    string a[3];
    a[0] = normalize(s1);
    a[1] = normalize(s2);
    a[2] = normalize(s3);
    vector<string> good;
    array<int,3> p = {0,1,2};
    sort(p.begin(), p.end());
    do {
        good.push_back(a[p[0]] + a[p[1]] + a[p[2]]);
    } while (next_permutation(p.begin(), p.end()));
    unordered_set<string> goodSet(good.begin(), good.end());
    int n;
    string line;
    if (!(cin >> n)) return 0;
    getline(cin, line); // consume endline
    for (int i = 0; i < n; ++i) {
        getline(cin, line);
        string b = normalize(line);
        if (goodSet.find(b) != goodSet.end())
            cout << "ACC\n";
        else
            cout << "WA\n";
    }
    return 0;
}