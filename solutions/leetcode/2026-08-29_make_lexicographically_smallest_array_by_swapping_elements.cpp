class Solution {
public:
    vector<int> lexSmallest(string& s, int limit) {
        // This is a placeholder; the actual problem uses vector<int>
        return {};
    }

    vector<int> lexSmallest(vector<int>& nums, int limit) {
        int n = nums.size();
        vector<int> sorted = nums;
        sort(sorted.begin(), sorted.end());
        
        // We need to find connected components where elements can be swapped.
        // Two elements can be swapped if |a - b| <= limit.
        // This forms a graph where edges exist between elements with difference <= limit.
        // Since we can swap any number of times, elements in the same connected component can be permuted freely.
        
        // The key insight: if we sort the array, then elements that are "close" (within limit) form groups.
        // Specifically, in the sorted array, if sorted[i] - sorted[i-1] <= limit, they are in the same component.
        // Wait, that's not quite right. The connectivity is transitive: if a~b and b~c, then a~c even if |a-c| > limit.
        
        // So in the sorted array, we can form groups where consecutive elements have difference <= limit.
        // Each such group is a connected component.
        
        // For each position in the original array, we need to assign the smallest available element from its component.
        
        // Step 1: Identify components in the sorted array.
        // A component is a maximal contiguous subsequence in sorted[] where consecutive differences are <= limit.
        
        // Step 2: For each component, collect the elements (they are already sorted).
        // Step 3: For each position i in the original array, determine which component nums[i] belongs to.
        //         Then assign the smallest remaining element from that component to position i.
        
        // But wait: the component membership is based on the values, not positions.
        // Two values a and b are in the same component if there's a chain of values where consecutive differences are <= limit.
        // In the sorted array, this means: start from sorted[0], extend the component as long as sorted[i] - sorted[i-1] <= limit.
        
        // Let's build the components:
        vector<vector<int>> components;
        int i = 0;
        while (i < n) {
            int j = i;
            while (j + 1 < n && sorted[j + 1] - sorted[j] <= limit) {
                j++;
            }
            vector<int> comp;
            for (int k = i; k <= j; k++) {
                comp.push_back(sorted[k]);
            }
            components.push_back(comp);
            i = j + 1;
        }
        
        // Now, for each value in the original array, we need to know which component it belongs to.
        // Since the components are contiguous in the sorted array, we can use binary search to find which component a value belongs to.
        
        // For each component, we'll maintain a pointer to the next available element (starting from the beginning, since we want the smallest).
        vector<int> ptr(components.size(), 0);
        
        // For each position in the original array, find the component of nums[i], and assign the next smallest element from that component.
        vector<int> result(n);
        
        // To find which component a value belongs to: 
        // The components partition the sorted array. We can precompute the range [start, end] for each component.
        vector<int> compStart(components.size()), compEnd(components.size());
        int idx = 0;
        for (int c = 0; c < components.size(); c++) {
            compStart[c] = idx;
            idx += components[c].size();
            compEnd[c] = idx - 1;
        }
        
        // For a given value v, find the component: binary search in sorted to find its position, then determine which component that position falls into.
        // Actually, since the components are contiguous in sorted[], we can binary search for the component by checking which [compStart[c], compEnd[c]] contains the position of v in sorted[].
        
        // But v might appear multiple times. We need to be careful.
        // Alternative: for each value v, we can find the component by binary searching for the first component where compEnd[c] >= position of v in sorted[].
        
        // Let's precompute for each index in sorted[], which component it belongs to.
        vector<int> compOfIndex(n);
        idx = 0;
        for (int c = 0; c < components.size(); c++) {
            for (int k = 0; k < components[c].size(); k++) {
                compOfIndex[idx] = c;
                idx++;
            }
        }
        
        // Now for each nums[i], find its position in sorted[] (using lower_bound), then get the component.
        // But if there are duplicates, lower_bound gives the first occurrence. That's fine because all duplicates of the same value are in the same component.
        
        for (int pos = 0; pos < n; pos++) {
            int v = nums[pos];
            // Find the first index in sorted[] where sorted[index] >= v
            int idxInSorted = lower_bound(sorted.begin(), sorted.end(), v) - sorted.begin();
            int c = compOfIndex[idxInSorted];
            // Assign the next available element from component c
            result[pos] = components[c][ptr[c]];
            ptr[c]++;
        }
        
        return result;
    }
};