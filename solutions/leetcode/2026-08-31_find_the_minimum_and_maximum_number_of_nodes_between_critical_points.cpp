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
        int first = -1, last = -1, prevCrit = -1;
        int minDist = INT_MAX;
        while (cur) {
            ListNode* nxt = cur->next;
            if (prev && nxt) {
                bool isMax = cur->val > prev->val && cur->val > nxt->val;
                bool isMin = cur->val < prev->val && cur->val < nxt->val;
                if (isMax || isMin) {
                    if (first == -1) {
                        first = idx;
                        prevCrit = idx;
                    } else {
                        minDist = min(minDist, idx - prevCrit);
                        prevCrit = idx;
                    }
                    last = idx;
                }
            }
            prev = cur;
            cur = nxt;
            ++idx;
        }
        if (first == -1 || first == last) return {-1, -1};
        int maxDist = last - first;
        return {minDist, maxDist};
    }
};