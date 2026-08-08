#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<string> names;
    vector<int> taxi, pizza, girl;

    for (int i = 0; i < n; i++) {
        int si;
        string name;
        cin >> si >> name;
        names.push_back(name);

        int taxiCount = 0, pizzaCount = 0, girlCount = 0;

        for (int j = 0; j < si; j++) {
            string phone;
            cin >> phone;

            if (phone[0] == phone[1] && phone[1] == phone[3] && phone[3] == phone[4] && phone[4] == phone[6] && phone[6] == phone[7]) {
                taxiCount++;
            } else if (phone[0] > phone[1] && phone[1] > phone[3] && phone[3] > phone[4] && phone[4] > phone[6] && phone[6] > phone[7]) {
                pizzaCount++;
            } else {
                girlCount++;
            }
        }

        taxi.push_back(taxiCount);
        pizza.push_back(pizzaCount);
        girl.push_back(girlCount);
    }

    int maxTaxi = *max_element(taxi.begin(), taxi.end());
    int maxPizza = *max_element(pizza.begin(), pizza.end());
    int maxGirl = *max_element(girl.begin(), girl.end());

    cout << "If you want to call a taxi, you should call: ";
    for (int i = 0; i < n; i++) {
        if (taxi[i] == maxTaxi) {
            cout << names[i];
            if (i < n - 1 && taxi[i + 1] == maxTaxi) {
                cout << ", ";
            }
        }
    }
    cout << "." << endl;

    cout << "If you want to order a pizza, you should call: ";
    for (int i = 0; i < n; i++) {
        if (pizza[i] == maxPizza) {
            cout << names[i];
            if (i < n - 1 && pizza[i + 1] == maxPizza) {
                cout << ", ";
            }
        }
    }
    cout << "." << endl;

    cout << "If you want to go to a cafe with a wonderful girl, you should call: ";
    for (int i = 0; i < n; i++) {
        if (girl[i] == maxGirl) {
            cout << names[i];
            if (i < n - 1 && girl[i + 1] == maxGirl) {
                cout << ", ";
            }
        }
    }
    cout << "." << endl;

    return 0;
}