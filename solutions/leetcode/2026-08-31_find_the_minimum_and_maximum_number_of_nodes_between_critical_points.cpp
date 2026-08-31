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
        if (!head) return {-1, -1};
        vector<int> critPos;
        int idx = 0;
        ListNode* prev = nullptr;
        ListNode* cur = head;
        while (cur) {
            ListNode* nxt = cur->next;
            if (prev && nxt) {
                if ((prev->val < cur->val && cur->val > nxt->val) ||
                    (prev->val > cur->val && cur->val < nxt->val)) {
                    critPos.push_back(idx);
                }
            }
            prev = cur;
            cur = nxt;
            ++idx;
        }
        if (critPos.size() < 2) return {-1, -1};
        int minDist = INT_MAX;
        for (size_t i = 1; i < critPos.size(); ++i) {
            minDist = min(minDist, critPos[i] - critPos[i-1]);
        }
        int maxDist = critPos.back() - critPos.front();
        return {minDist, maxDist};
    }
};