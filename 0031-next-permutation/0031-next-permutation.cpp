class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int idx;
        for(int i=nums.size()-2;i>=0;i--){
            if(nums[i]<nums[i+1]){
                idx=i;
                break;
            }
        }//found the break point to find from where it is possible to find the permuation
        if (idx == -1) {
            reverse(nums.begin(), nums.end());
            return;
        }

        for(int i=nums.size()-1;i>idx;i--){
            if(nums[i]>nums[idx]){
                swap(nums[i],nums[idx]);
                break;
            }
        }//swapped with the first element that is greater than the break point to find the next smallest permuation


        reverse(nums.begin()+idx+1,nums.end());

    }
};