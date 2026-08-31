#include <bits/stdc++.h>
using namespace std;

/**
 * Definition for singly-linked list.
 */
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
        int idx = 0;
        ListNode* prev = nullptr;
        ListNode* cur = head;
        ListNode* next = cur->next;
        int firstPos = -1, prevPos = -1;
        int minDist = INT_MAX;
        int maxDist = -1;
        while (cur && next) {
            // check critical point for cur (needs prev and next)
            if (prev) {
                bool isMax = (cur->val > prev->val) && (cur->val > next->val);
                bool isMin = (cur->val < prev->val) && (cur->val < next->val);
                if (isMax || isMin) {
                    if (firstPos == -1) {
                        firstPos = idx;
                    } else {
                        minDist = min(minDist, idx - prevPos);
                        maxDist = max(maxDist, idx - firstPos);
                    }
                    prevPos = idx;
                }
            }
            // advance
            prev = cur;
            cur = next;
            next = next->next;
            ++idx;
        }
        if (firstPos == -1 || prevPos == firstPos) return {-1, -1};
        return {minDist, maxDist};
    }
};