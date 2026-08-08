class Solution {
public:
    vector<int> sequentialDigits(int low, int high) {
        vector<int> result;
        for (int digits = to_string(low).size(); digits <= to_string(high).size(); digits++) {
            for (int start = 1; start <= 9 - digits + 1; start++) {
                int num = 0;
                for (int i = 0; i < digits; i++) {
                    num = num * 10 + start + i;
                }
                if (num >= low && num <= high) {
                    result.push_back(num);
                }
            }
        }
        return result;
    }
};