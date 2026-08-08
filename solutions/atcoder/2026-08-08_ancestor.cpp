#include <iostream>
using namespace std;

int main() {
    int n;
    cin >> n;
    int p[n];
    for (int i = 0; i < n - 1; i++) {
        cin >> p[i];
    }
    int count = 0;
    int x = n - 1;
    while (x > 0) {
        x = p[x - 1] - 1;
        count++;
    }
    cout << count << endl;
    return 0;
}