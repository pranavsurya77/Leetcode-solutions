class Solution {
public:
    int minDays(vector<int>& bloomDay, int m, int k) {
        if ((long long)m * k > bloomDay.size()) return -1;

        int low = 1;
        int high = *max_element(bloomDay.begin(), bloomDay.end());
        int ans = -1;

        while (low <= high) {
            int mid = low + (high - low) / 2;

            //so now we check if we can make the required no of boquets with the current mid we have if yes it is a possible ans and we try to find the min by going to the left half if not then the no of days is not enough and we need more days so we go into the right half

            // Check if we can make m bouquets by day = mid
            int bouquets = 0, adjacent = 0;
            for (int i = 0; i < bloomDay.size(); i++) {
                if (bloomDay[i] <= mid) {
                    adjacent++;
                    if (adjacent == k) {
                        bouquets++;
                        adjacent = 0;
                    }
                } else {
                    adjacent = 0;
                }
            }

            if (bouquets >= m) {
                ans = mid;
                high = mid - 1; // Try to find a smaller valid day
            } else {
                low = mid + 1; // Need more days
            }
        }

        return ans;
    }
};
