#include <iostream>
#include <string>
#include <cmath>

using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

long long lcm(long long a, long long b) {
    return (a * b) / gcd(a, b);
}

long long get_product(const string& num) {
    long long product = 1;
    for (char c : num) {
        if (c == '0') return -1;
        product *= (c - '0');
    }
    return product;
}

string smallest_divisible_digit_product_ii(string num, long long t) {
    long long n = num.size();
    long long limit = stoll(num) + 1;

    while (true) {
        string str = to_string(limit);
        if (str.size() > n) break;
        if (get_product(str) != -1 && get_product(str) % t == 0) {
            return str;
        }
        limit++;
    }

    while (true) {
        string str = to_string(limit);
        if (get_product(str) != -1 && get_product(str) % t == 0) {
            return str;
        }
        limit++;
    }
}

int main() {
    string num;
    long long t;

    num = "1234";
    t = 256;
    cout << smallest_divisible_digit_product_ii(num, t) << endl;

    num = "12355";
    t = 50;
    cout << smallest_divisible_digit_product_ii(num, t) << endl;

    num = "11111";
    t = 26;
    cout << smallest_divisible_digit_product_ii(num, t) << endl;

    return 0;
}