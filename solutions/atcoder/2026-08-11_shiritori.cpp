#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<string> w(n);
    for (int i = 0; i < n; i++) {
        cin >> w[i];
    }
    set<string> st;
    for (int i = 0; i < n; i++) {
        if (st.find(w[i]) != st.end()) {
            cout << "No" << endl;
            return 0;
        }
        st.insert(w[i]);
    }
    for (int i = 1; i < n; i++) {
        if (w[i-1].back() != w[i].front()) {
            cout << "No" << endl;
            return 0;
        }
    }
    cout << "Yes" << endl;
    return 0;
}