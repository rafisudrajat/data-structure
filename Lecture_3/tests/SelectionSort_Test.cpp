#include "SelectionSort.h"
#include <gtest/gtest.h>
using namespace AlgorithmPractice;

// Test case to check if insertion sort works correctly
TEST(SelectionSortRecursive, ShouldSortArray) {
    // Preparation
    int unsortedArray[5] = {9,10,6,4,2};

    // Call
    selectionSortRecursive(unsortedArray,4);    

    // Assertion
    // Verify that the array is sorted correctly
    for (size_t i = 1; i < 5; ++i) {
        EXPECT_LE(unsortedArray[i - 1], unsortedArray[i]);
    }
}

// Test case to check if insertion sort works correctly
TEST(SelectionSortIterative, ShouldSortArray) {
    // Preparation
    int unsortedArray[] = {5, 3, 8, 1, 2};

    // Call
    selectionSortIterative(unsortedArray,5);

    // Assertion
    // Verify that the array is sorted correctly
    for (size_t i = 1; i < 5; ++i) {
        EXPECT_LE(unsortedArray[i - 1], unsortedArray[i]);
    }
}