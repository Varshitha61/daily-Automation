#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    vector<int> nodesBetweenCriticalPoints(ListNode* head) {
        vector<int> crit;
        ListNode* prev = nullptr;
        ListNode* cur = head;
        int idx = 0;
        while (cur) {
            ListNode* nxt = cur->next;
            if (prev && nxt) {
                if ((cur->val > prev->val && cur->val > nxt->val) ||
                    (cur->val < prev->val && cur->val < nxt->val)) {
                    crit.push_back(idx);
                }
            }
            prev = cur;
            cur = nxt;
            ++idx;
        }
        if (crit.size() < 2) return {-1, -1};
        int minDist = INT_MAX;
        for (size_t i = 1; i < crit.size(); ++i) {
            minDist = min(minDist, crit[i] - crit[i - 1]);
        }
        int maxDist = crit.back() - crit.front();
        return {minDist, maxDist};
    }
};