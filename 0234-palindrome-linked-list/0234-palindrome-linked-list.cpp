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
    bool isPalindrome(ListNode* head) {
        
        //first find middle than reverse the other half and then compare
        ListNode* slow=head;
        ListNode* fast=head;

        while(fast!=NULL && fast->next!=NULL){
            slow=slow->next;
            fast=fast->next->next;
        }

        //now slow is the middle
        ListNode* node=NULL;
        while(slow!=NULL){
            ListNode* temp=slow->next;
            slow->next=node;
            node=slow;
            slow=temp;
        }
        

        //now the last node stays at node
        while(node!=NULL){
            if(node->val!=head->val) return false;
            node=node->next;
            head=head->next;
        }
        return true;
        
    }
};