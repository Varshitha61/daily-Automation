#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minSumOfLengths(vector<int>& arr, int target) {
        int n = arr.size();
        const int INF = 1e9;
        vector<int> best(n, INF);
        long long sum = 0;
        int left = 0;
        int minLenSoFar = INF;
        int answer = INF;
        for (int right = 0; right < n; ++right) {
            sum += arr[right];
            while (sum > target && left <= right) {
                sum -= arr[left];
                ++left;
            }
            if (sum == target) {
                int len = right - left + 1;
                if (left > 0 && best[left - 1] != INF) {
                    answer = min(answer, best[left - 1] + len);
                }
                minLenSoFar = min(minLenSoFar, len);
            }
            best[right] = minLenSoFar;
        }
        return answer == INF ? -1 : answer;
    }
};