class Solution {
public:
    int smallestDivisible(int n, int t) {
        while (true) {
            long long product = 1;
            int temp = n;
            while (temp > 0) {
                product *= temp % 10;
                temp /= 10;
            }
            if (product % t == 0) return n;
            n++;
        }
    }
};