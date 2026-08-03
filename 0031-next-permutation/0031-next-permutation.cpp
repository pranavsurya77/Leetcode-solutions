class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        //first find the break point then find the next largest element and replace it
        // with the break point element. then reverse the rest.

        //find break point
        int idx=-1;
        for(int i=nums.size()-1;i>0;i--){
            if(nums[i]>nums[i-1]){
                idx=i-1;
                break;
            }
        }

        if(idx==-1){
            reverse(nums.begin(),nums.end());
            return;
        }

        //find the next greater element to break point element
        //and swap them
        int last=nums.size()-1;
        while(last>idx){
            if(nums[last]>nums[idx]){
                swap(nums[last],nums[idx]);
                break;
            }
            last--;
        }

        //now reverse from break point till last to find just the next greatest element
        reverse(nums.begin()+idx+1,nums.end());
    }
};