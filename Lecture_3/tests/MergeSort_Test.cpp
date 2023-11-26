#include "MergeSort.h"
#include <gtest/gtest.h>
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