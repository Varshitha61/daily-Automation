#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    vector<vector<char>> board(9, vector<char>(9));
    for (int i = 0; i < 9; i++) {
        string s;
        cin >> s;
        for (int j = 0; j < 3; j++) {
            board[i][j] = s[j];
        }
        if (i % 3 != 2) {
            cin >> s;
            for (int j = 3; j < 6; j++) {
                board[i][j] = s[j - 3];
            }
            cin >> s;
            for (int j = 6; j < 9; j++) {
                board[i][j] = s[j - 6];
            }
        }
    }
    int x, y;
    cin >> x >> y;
    x--; y--;
    int subgridX = x / 3;
    int subgridY = y / 3;
    bool canMoveAnywhere = true;
    for (int i = subgridX * 3; i < subgridX * 3 + 3; i++) {
        for (int j = subgridY * 3; j < subgridY * 3 + 3; j++) {
            if (board[i][j] == '.') {
                canMoveAnywhere = false;
            }
        }
    }
    if (canMoveAnywhere) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] == '.') {
                    board[i][j] = '!';
                }
            }
        }
    } else {
        for (int i = subgridX * 3; i < subgridX * 3 + 3; i++) {
            for (int j = subgridY * 3; j < subgridY * 3 + 3; j++) {
                if (board[i][j] == '.') {
                    board[i][j] = '!';
                }
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << board[i][j];
            if ((j + 1) % 3 == 0 && j != 8) {
                cout << " ";
            }
        }
        cout << endl;
        if ((i + 1) % 3 == 0 && i != 8) {
            cout << endl;
        }
    }
    return 0;
}