#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

std::vector<int> arrayRankTransform(std::vector<int>& arr) {
    std::vector<int> sorted_arr = arr;
    std::sort(sorted_arr.begin(), sorted_arr.end());
    std::unordered_map<int, int> rank_map;
    int rank = 1;
    for (int i = 0; i < sorted_arr.size(); i++) {
        if (rank_map.find(sorted_arr[i]) == rank_map.end()) {
            rank_map[sorted_arr[i]] = rank;
            rank++;
        }
    }
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = rank_map[arr[i]];
    }
    return arr;
}

int main() {
    std::vector<int> arr1 = {40, 10, 20, 30};
    std::vector<int> result1 = arrayRankTransform(arr1);
    for (int i = 0; i < result1.size(); i++) {
        std::cout << result1[i] << " ";
    }
    std::cout << std::endl;

    std::vector<int> arr2 = {100, 100, 100};
    std::vector<int> result2 = arrayRankTransform(arr2);
    for (int i = 0; i < result2.size(); i++) {
        std::cout << result2[i] << " ";
    }
    std::cout << std::endl;

    std::vector<int> arr3 = {37, 12, 28, 9, 100, 56, 80, 5, 12};
    std::vector<int> result3 = arrayRankTransform(arr3);
    for (int i = 0; i < result3.size(); i++) {
        std::cout << result3[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}