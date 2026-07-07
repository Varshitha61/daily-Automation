#include <iostream>
using namespace std;

int solveMeFirst(int a, int b) {
    return a + b;
}

int main() {
    int a, b;
    cin >> a >> b;
    int sum = solveMeFirst(a, b);
    cout << sum;
    return 0;
}