/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        if(head==NULL) return NULL;
        ListNode* even=head->next;
        ListNode* odd=head;

        ListNode* initial=even;

        ListNode* tempEven=even;
        ListNode* tempOdd=odd;
        
        while(odd->next!=NULL && even->next!=NULL){
            odd=odd->next->next;
            even=even->next->next;

            tempOdd->next=odd;
            tempEven->next=even;

            tempOdd=odd;
            tempEven=even;
        }
        odd->next=initial;
        return head;
    }
};