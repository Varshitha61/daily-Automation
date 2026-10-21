#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<string> names;
    vector<int> taxiCnt, pizzaCnt, girlCnt;
    for (int i = 0; i < n; ++i) {
        int s;
        string name;
        cin >> s >> name;
        names.push_back(name);
        int t = 0, p = 0, g = 0;
        for (int j = 0; j < s; ++j) {
            string num;
            cin >> num;
            string d;
            for (char c : num) if (c != '-') d.push_back(c);
            bool isTaxi = true;
            for (int k = 1; k < 6; ++k) if (d[k] != d[0]) { isTaxi = false; break; }
            bool isPizza = true;
            for (int k = 1; k < 6; ++k) if (!(d[k-1] > d[k])) { isPizza = false; break; }
            if (isTaxi) ++t;
            else if (isPizza) ++p;
            else ++g;
        }
        taxiCnt.push_back(t);
        pizzaCnt.push_back(p);
        girlCnt.push_back(g);
    }
    int maxTaxi = *max_element(taxiCnt.begin(), taxiCnt.end());
    int maxPizza = *max_element(pizzaCnt.begin(), pizzaCnt.end());
    int maxGirl = *max_element(girlCnt.begin(), girlCnt.end());

    vector<string> bestTaxi, bestPizza, bestGirl;
    for (int i = 0; i < n; ++i) {
        if (taxiCnt[i] == maxTaxi) bestTaxi.push_back(names[i]);
        if (pizzaCnt[i] == maxPizza) bestPizza.push_back(names[i]);
        if (girlCnt[i] == maxGirl) bestGirl.push_back(names[i]);
    }

    auto printLine = [&](const string& prefix, const vector<string>& list) {
        cout << prefix;
        for (size_t i = 0; i < list.size(); ++i) {
            if (i) cout << ", ";
            cout << list[i];
        }
        cout << "." << "\n";
    };

    printLine("If you want to call a taxi, you should call: ", bestTaxi);
    printLine("If you want to order a pizza, you should call: ", bestPizza);
    printLine("If you want to go to a cafe with a wonderful girl, you should call: ", bestGirl);
    return 0;
}