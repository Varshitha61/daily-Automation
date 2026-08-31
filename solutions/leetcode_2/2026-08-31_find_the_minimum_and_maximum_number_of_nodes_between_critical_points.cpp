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
        int idx = 0;
        ListNode* prev = nullptr;
        ListNode* curr = head;
        ListNode* next = curr->next;
        int firstPos = -1, prevPos = -1;
        int minDist = INT_MAX;
        int lastPos = -1;
        while (curr && next) {
            // check critical point for curr (must have prev and next)
            if (prev) {
                bool isMax = (curr->val > prev->val) && (curr->val > next->val);
                bool isMin = (curr->val < prev->val) && (curr->val < next->val);
                if (isMax || isMin) {
                    if (firstPos == -1) {
                        firstPos = idx;
                    } else {
                        minDist = min(minDist, idx - prevPos);
                    }
                    prevPos = idx;
                    lastPos = idx;
                }
            }
            // move forward
            prev = curr;
            curr = next;
            next = next->next;
            ++idx;
        }
        if (firstPos == -1 || firstPos == prevPos) return {-1, -1};
        int maxDist = lastPos - firstPos;
        return {minDist, maxDist};
    }
};