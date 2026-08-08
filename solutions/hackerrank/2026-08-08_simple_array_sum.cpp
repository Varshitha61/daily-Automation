#include <iostream>
using namespace std;

int simpleArraySum(int ar[], int n) {
    int sum = 0;
    for(int i = 0; i < n; i++) {
        sum += ar[i];
    }
    return sum;
}

int main() {
    int n;
    cin >> n;
    int ar[n];
    for(int i = 0; i < n; i++) {
        cin >> ar[i];
    }
    int result = simpleArraySum(ar, n);
    cout << result << endl;
    return 0;
}