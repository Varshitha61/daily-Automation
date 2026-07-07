class Solution {
public:
    int multiplyDigits(int n) {
        string str = to_string(n);
        string x;
        int sum = 0;
        
        for (char c : str) {
            if (c != '0') {
                x += c;
                sum += c - '0';
            }
        }
        
        if (x.empty()) {
            return 0;
        }
        
        return stoi(x) * sum;
    }
};