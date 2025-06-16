class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        long long low = 1;
        long long high = *max_element(piles.begin(), piles.end());
        long long result = high;

        while (low <= high) {
            long long mid = low + (high - low) / 2;
            long long timeTaken = 0;

            for (int i = 0; i < piles.size(); i++) {
                timeTaken += (piles[i] + mid - 1) / mid;  // integer ceil
            }

            if (timeTaken <= h) {
                result = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        return (int)result;
    }
};
