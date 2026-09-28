#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    if(!(cin >> s)) return 0;
    int bal = 0;
    for(char c: s){
        if(c=='(') bal++;
        else if(c==')') bal--;
        if(bal<0){
            cout << "NO";
            return 0;
        }
    }
    cout << (bal==0?"YES":"NO");
    return 0;
}