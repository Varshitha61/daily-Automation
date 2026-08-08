#include <iostream>
#include <string>

using namespace std;

int main() {
    string s;
    cin >> s;
    int n = s.size();
    int r = 1, c = 1;
    for (int i = 0; i < n; i++) {
        if (s[i] == '0') {
            cout << r << " " << c << endl;
            c += 2;
            if (c > 3) {
                c = 1;
                r += 2;
                if (r > 3) r = 1;
            }
        } else {
            cout << r << " " << c << endl;
            r += 2;
            if (r > 3) {
                r = 1;
                c += 2;
                if (c > 3) c = 1;
            }
        }
    }
    return 0;
}