#include <bits/stdc++.h>
using namespace std;

bool aliceWins(int n) {
    int required = 10;
    bool aliceTurn = true; // true for Alice, false for Bob
    while (n >= required && required > 0) {
        n -= required;
        required--;
        aliceTurn = !aliceTurn;
    }
    // player whose turn it is cannot move and loses
    // if it's Alice's turn and she cannot move, she loses -> return false
    // otherwise Bob loses -> Alice wins
    return !aliceTurn;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n;
    bool first = true;
    while (cin >> n) {
        bool res = aliceWins((int)n);
        cout << (res ? "true" : "false");
        if (!cin.eof()) cout << "\n";
    }
    return 0;
}