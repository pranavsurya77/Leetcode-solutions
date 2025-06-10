class Solution {
public:
    int findMin(vector<int>& nums) {
        int n=nums.size();
        int low=0;
        int high=n-1;
        int minNo=INT_MAX;
        while(low<=high){
            int mid=low+((high-low)/2);
            

            if(nums[low]<=nums[high]){
                minNo=min(minNo,nums[low]);
                break;
            }

            if(nums[low]<=nums[mid]){
                minNo=min(minNo,nums[low]);
                low=mid+1;
            }else{
                minNo=min(minNo,nums[mid]);
                high=mid-1;
            }

        }
        return minNo;
    }
};