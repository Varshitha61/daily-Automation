#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    int pos = 0;
    while (true) {
        if (a[pos] == 0) {
            break;
        }
        for (int i = 0; i < n; i++) {
            if (a[i] > 0) {
                a[i]--;
            }
        }
        pos = (pos + 1) % n;
    }
    cout << pos + 1 << endl;
    return 0;
}