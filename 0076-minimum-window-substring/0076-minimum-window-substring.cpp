class Solution {
public:
    string minWindow(string s, string t) {
        int count=t.size(); //how many more valid char we need
        vector<int> hash(126,0);
        int left=0,right=0;
        int d=INT_MAX,h=0;
        //d is the length of the string and h is the starting point
        for(char c:t){hash[c]++;}
        while(right<s.size()){
            if(hash[s[right++]]-->0) count--; //so we reduce the char count from hash and if the count is still greater than 0 before decre then it is is a valid counter
            while(count==0){//means it is a valid substr now we try to reduce the window size
                if(right-left<d) d=right-(h=left);
                //the aboce line checks if the new substr is smaller and stores the starting point in head nad the length in d
                hash[s[left]]++;
                if(hash[s[left]]>0) count++; //becomes invalid
                left++;
                //we reduce the window then we check if a valid char became invalid if yes then we increase the count.
            }
        }
        return d==INT_MAX?"":s.substr(h,d);
    }
};