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
        ListNode* curr = head;
        ListNode* next = head->next;
        int firstCrit = -1, prevCrit = -1;
        int minDist = INT_MAX, maxDist = -1;
        while (curr && next) {
            // check if curr is a critical point (needs both prev and next)
            if (prev) {
                if ((curr->val > prev->val && curr->val > next->val) ||
                    (curr->val < prev->val && curr->val < next->val)) {
                    if (firstCrit == -1) {
                        firstCrit = idx;
                    } else {
                        int dist = idx - prevCrit;
                        minDist = min(minDist, dist);
                        maxDist = max(maxDist, idx - firstCrit);
                    }
                    prevCrit = idx;
                }
            }
            // move forward
            prev = curr;
            curr = next;
            next = next->next;
            ++idx;
        }
        if (minDist == INT_MAX) return {-1, -1};
        return {minDist, maxDist};
    }
};