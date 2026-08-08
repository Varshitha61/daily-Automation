class Solution {
public:
    int findGCD(vector<int>& nums) {
        int minNum = *min_element(nums.begin(), nums.end());
        int maxNum = *max_element(nums.begin(), nums.end());
        
        return gcd(minNum, maxNum);
    }
    
    int gcd(int a, int b) {
        if (b == 0) return a;
        return gcd(b, a % b);
    }
};