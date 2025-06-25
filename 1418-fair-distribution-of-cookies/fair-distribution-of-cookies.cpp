class Solution {
public:
    bool backtrack(vector<int>& cookies, vector<int>& children, int index, int k, int limit) {
        if (index == cookies.size()) return true;

        for (int i = 0; i < k; i++) {
            if (children[i] + cookies[index] <= limit) {
                children[i] += cookies[index];
                if (backtrack(cookies, children, index + 1, k, limit))
                    return true;
                children[i] -= cookies[index];
            }
            // optimization: if a child hasn't received any cookies yet,
            // no need to try giving the same cookie to other "empty" children
            if (children[i] == 0) break;
        }

        return false;
    }

    int distributeCookies(vector<int>& cookies, int k) {
        int low = *max_element(cookies.begin(), cookies.end());
        int high = accumulate(cookies.begin(), cookies.end(), 0);
        int ans = high;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            vector<int> children(k, 0);
            if (backtrack(cookies, children, 0, k, mid)) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        return ans;
    }
};
