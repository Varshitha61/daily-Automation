#include <bits/stdc++.h>
using namespace std;

bool aliceWins(int n) {
    int need = 10;
    int turn = 0; // 0 - Alice, 1 - Bob
    while (need > 0 && n >= need) {
        n -= need;
        need--;
        turn ^= 1;
    }
    // turn is the player who is about to move but cannot
    return turn == 1; // Alice wins if Bob cannot move
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