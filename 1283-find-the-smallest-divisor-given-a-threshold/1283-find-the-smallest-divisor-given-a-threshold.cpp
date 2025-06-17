class Solution {
public:
    int smallestDivisor(vector<int>& nums, int threshold) {
        int low=1;
        int high=*max_element(nums.begin(),nums.end());
        int result=high;
        while(low<=high){
            int mid=low+((high-low)/2);

            //now we check if keeping the mid as the number to be divided by
            int thresh=0;
            for(int num:nums){
                thresh+=(num+mid-1)/mid;
            }
            if(thresh<=threshold){
                result=mid;
                high=mid-1;//since we got one of the possible ans we try to find a lower no as result is possible
            }else{
                low=mid+1;
            }
        }
        return result;
    }
};