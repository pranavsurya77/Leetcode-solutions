class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int> hash;

        for(int i=0;i<nums.size();i++){
            int current=nums[i];
            int needed=target-nums[i];

            if(hash.find(needed)!=hash.end()){
                return {i,hash[needed]};
            }else{
                hash[nums[i]]=i;
            }
        }
        return {};
    }
};