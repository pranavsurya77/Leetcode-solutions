/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:

    // void preorder(TreeNode* root, vector<int>& result){
    //     if(root == nullptr){
    //         return;
    //     }
    //     result.push_back(root->val);
    //     preorder(root->left,result);
    //     preorder(root->right,result); 
    // }

    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> result;
        
        if(root==nullptr){
            return {};
        }
        stack<TreeNode*> st;
        st.push(root);

        while(!st.empty()){
            TreeNode* node=st.top();
            st.pop();
            result.push_back(node->val);

            if(node->right!=nullptr) st.push(node->right); //right is before left bcz we are using a stack
            if(node->left!=nullptr) st.push(node->left);
        }
        return result;
    }
};