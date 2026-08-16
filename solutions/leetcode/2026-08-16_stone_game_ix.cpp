#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    bool stoneGameIX(vector<int>& stones) {
        int n = stones.size();
        vector<int> count(3);
        for (int stone : stones) {
            count[stone % 3]++;
        }
        int sum = 0;
        for (int stone : stones) {
            sum += stone;
        }
        if (sum % 3 == 0) {
            return count[0] > 2;
        }
        if (count[0] == 0) {
            return count[1] != 0 && count[2] != 0;
        }
        return count[1] > count[2] + 2 || count[2] > count[1] + 2;
    }
};

int main() {
    Solution solution;
    vector<int> stones1 = {2, 1};
    vector<int> stones2 = {2};
    vector<int> stones3 = {5, 1, 2, 4, 3};
    cout << boolalpha << solution.stoneGameIX(stones1) << endl;  // Output: true
    cout << boolalpha << solution.stoneGameIX(stones2) << endl;  // Output: false
    cout << boolalpha << solution.stoneGameIX(stones3) << endl;  // Output: false
    return 0;
}