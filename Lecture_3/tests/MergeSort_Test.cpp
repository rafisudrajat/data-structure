#include "MergeSort.h"
#include <gtest/gtest.h>
#include <vector>
using namespace AlgorithmPractice;

// Test case to check if insertion sort works correctly
TEST(MergeSort, ShouldSortArray) {
    // Preparation
    int unsortedArray[] = {5, 3, 8, 1, 2};

    // Call
    numericalMergeSort(unsortedArray,0,4);

    // Assertion
    // Verify that the array is sorted correctly
    for (size_t i = 1; i < 5; ++i) {
        EXPECT_LE(unsortedArray[i - 1], unsortedArray[i]);
    }
}

TEST(MergeSort, ShouldSortVector){
    // Preparation
    std::vector<int> unsortedVector={5, 3, 8, 1, 2};

    //Call
    numericalMergeSort(unsortedVector,0,4);

    // Assertion
    // Verify that the array is sorted correctly
    for (size_t i = 1; i < 5; ++i) {
        EXPECT_LE(unsortedVector[i - 1], unsortedVector[i]);
    }
}