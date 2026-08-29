class Solution {
public:
    vector<int> lexSmallest(string& nums, int limit) {
        // This is a placeholder - the actual problem uses vector<int>
        return {};
    }
    
    vector<int> lexSmallest(vector<int>& nums, int limit) {
        int n = nums.size();
        vector<int> sorted = nums;
        sort(sorted.begin(), sorted.end());
        
        // We need to find connected components where elements can be swapped.
        // Two elements can be swapped if |a - b| <= limit.
        // This forms a graph where edges exist between elements with difference <= limit.
        // Since we can swap any number of times, elements in the same connected component can be rearranged arbitrarily.
        
        // Key insight: If we sort the array, then elements that are "close" (within limit) form groups.
        // Specifically, if sorted[i] and sorted[i+1] differ by more than limit, then there's a break.
        // All elements in a contiguous segment of the sorted array where consecutive differences are <= limit form a connected component.
        
        // For each position in the original array, we need to place the smallest available element from its component.
        
        // Step 1: Identify components in the sorted array.
        // A component is a maximal contiguous subarray in sorted where consecutive elements differ by <= limit.
        
        vector<int> compId(n, -1);
        int compCount = 0;
        for (int i = 0; i < n; i++) {
            if (i == 0 || sorted[i] - sorted[i-1] > limit) {
                compCount++;
            }
            compId[i] = compCount - 1;
        }
        
        // Step 2: For each component, collect the sorted values (they're already sorted in 'sorted').
        // We need to assign the smallest available value from each component to the positions that belong to that component.
        
        // For each position in the original array, determine which component it belongs to.
        // Wait - the component is determined by the VALUE, not the position.
        // Actually, the connectivity is based on values. Two values a and b can be swapped if |a-b| <= limit.
        // So the components are groups of values where consecutive sorted values differ by <= limit.
        
        // For each position i in the original array, nums[i] belongs to component compId[rank of nums[i] in sorted].
        // But we need to be careful with duplicates.
        
        // Better approach:
        // 1. Sort the array.
        // 2. Identify component boundaries in the sorted array.
        // 3. For each component, we have a sorted list of values.
        // 4. For each position in the original array, we need to know which component its value belongs to.
        // 5. Then, for each component, we assign the smallest remaining value to the earliest position that belongs to that component.
        
        // Let's use a different approach:
        // - Sort the array to get sorted values.
        // - Identify components in the sorted array.
        // - For each component, we have a range [start, end) in the sorted array.
        // - For each position i in the original array, find which component nums[i] belongs to.
        // - Then, for each component, collect all positions that have values in that component.
        // - Sort those positions, and assign the sorted values of that component to those positions in order.
        
        // To find which component a value belongs to, we can use binary search on the sorted array.
        // But we need to handle duplicates carefully.
        
        // Alternative: Use a map from value to component. But values can be up to 1e9 and there can be duplicates.
        
        // Let's use the following:
        // - Create a sorted copy.
        // - For each index in the sorted array, determine its component.
        // - For the original array, for each element, find its component by binary searching in the sorted array.
        //   But with duplicates, we need to be careful.
        
        // Actually, since the sorted array is sorted, and components are contiguous in the sorted array,
        // we can for each value in the original array, find the range in the sorted array where that value appears,
        // and then determine the component.
        
        // Simpler: 
        // - For each position i in the original array, we want to assign the smallest available value from the same component.
        // - We can process positions from left to right.
        // - For each position, we need to find the smallest value in the same component that hasn't been used yet.
        
        // Let's use a priority queue or a pointer for each component.
        
        // Step 1: Sort the array.
        vector<int> sorted = nums;
        sort(sorted.begin(), sorted.end());
        
        // Step 2: Determine component for each index in the sorted array.
        vector<int> compOfSorted(n, 0);
        int comp = 0;
        compOfSorted[0] = 0;
        for (int i = 1; i < n; i++) {
            if (sorted[i] - sorted[i-1] > limit) {
                comp++;
            }
            compOfSorted[i] = comp;
        }
        
        // Step 3: For each component, collect the values (they are in sorted order in 'sorted').
        // We'll use a pointer for each component to track the next available value.
        vector<int> compStart(comp + 1, 0);
        vector<int> compEnd(comp + 1, 0);
        for (int i = 0; i < n; i++) {
            int c = compOfSorted[i];
            if (compStart[c] == 0 && i > 0) {
                // This is not the first element of this component
            }
            compEnd[c] = i + 1;
        }
        for (int c = 0; c <= comp; c++) {
            if (c == 0) {
                compStart[c] = 0;
            } else {
                compStart[c] = compEnd[c-1];
            }
        }
        
        // Step 4: For each position in the original array, determine its component.
        // We need to map each value to a component. Since the sorted array has the values in order,
        // and components are contiguous, we can for each value in the original array,
        // find the first occurrence in the sorted array and get the component.
        
        // But with duplicates, multiple values can map to the same component.
        // We need to assign components to positions in the original array.
        
        // Let's create a vector of pairs (value, original_index) and sort by value.
        // Then, for each element in this sorted list, we can determine its component based on its position in the sorted array.
        
        vector<pair<int, int>> valIdx;
        for (int i = 0; i < n; i++) {
            valIdx.push_back({nums[i], i});
        }
        sort(valIdx.begin(), valIdx.end());
        
        // Now, for each element in valIdx (which is sorted by value), determine its component.
        // The component is determined by the gap between consecutive values in the sorted array.
        vector<int> compOfOrig(n, 0);
        for (int i = 0; i < n; i++) {
            if (i == 0) {
                compOfOrig[valIdx[i].second] = 0;
            } else {
                if (valIdx[i].first - valIdx[i-1].first > limit) {
                    compOfOrig[valIdx[i].second] = compOfOrig[valIdx[i-1].second] + 1;
                } else {
                    compOfOrig[valIdx[i].second] = compOfOrig[valIdx[i-1].second];
                }
            }
        }
        
        // Step 5: For each component, collect the positions (original indices) that belong to it.
        vector<vector<int>> compPositions(comp + 1);
        for (int i = 0; i < n; i++) {
            compPositions[compOfOrig[i]].push_back(i);
        }
        
        // Step 6: For each component, sort the positions and assign the sorted values of that component to those positions.
        // The sorted values of component c are sorted[compStart[c] ... compEnd[c]-1].
        vector<int> result(n);
        for (int c = 0; c <= comp; c++) {
            sort(compPositions[c].begin(), compPositions[c].end());
            for (int j = 0; j < compPositions[c].size(); j++) {
                int pos = compPositions[c][j];
                result[pos] = sorted[compStart[c] + j];
            }
        }
        
        return result;
    }
};