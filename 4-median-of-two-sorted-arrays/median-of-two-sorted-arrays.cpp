class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int n1=nums1.size();
        int n2=nums2.size();

        if(n1>n2) return findMedianSortedArrays(nums2,nums1);//this is done bcz we always want the smaller first
        int n=n1+n2;
        int leftPartition=(n1+n2+1)/2;
        int low=0;
        int high=n1;
        while(low<=high){
            int mid1=(low+high) >> 1;// we calcualte the mid and check if mid no of elements from the first
            //array satisfies the conditions
            int l1=INT_MIN,l2=INT_MIN;
            int r1=INT_MAX,r2=INT_MAX;
            int mid2=leftPartition-mid1;

            //now we give values to l1 and all other variables
            if(mid1<n1){
                r1=nums1[mid1];
            }
            if(mid2<n2) r2=nums2[mid2];
            if(mid1-1>=0) l1=nums1[mid1-1];
            if(mid2-1>=0) l2=nums2[mid2-1];

            if(l1<=r2 && l2<=r1){
                if(n%2==0){
                    return ((double)max(l1,l2)+min(r1,r2))/2.0;
                }else{
                    return max(l1,l2);
                }
            }else if(l1>r2){
                high=mid1-1;
            }else{
                low=mid1+1;
            }
        }
        return 0;
    }
};