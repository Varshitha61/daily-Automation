#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> tasks(n);
    int sum = 0;
    for (int i = 0; i < n; i++) {
        cin >> tasks[i];
        sum += tasks[i];
    }
    int avg = sum / n;
    int rem = sum % n;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        if (tasks[i] > avg) {
            ans += tasks[i] - avg;
        }
    }
    cout << ans << endl;
    return 0;
}