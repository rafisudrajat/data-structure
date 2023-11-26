#include "PermutationSort.h"
#include <gtest/gtest.h>
using namespace AlgorithmPractice;

// Test case to check if the function checkIfArrayIsSortedAscending return correct value
TEST(CheckIfArrayIsSortedAscending, ShouldReturnTrueIfSortedAscending) {
    // Preparation
    int unsortedArray[] = {1,2,3,4,5,6,7,8};

    // Call
    bool arrayIsSorted= checkIfArrayIsSortedAscending(unsortedArray, 8);

    //Assertion
    EXPECT_TRUE(arrayIsSorted);
}

// Test case to check if the function checkIfArrayIsSortedAscending return correct value
TEST(CheckIfArrayIsSortedAscending, ShouldReturnFalseIfNotSortedAscending) {
    // Preparation
    int unsortedArray[] = {1,2,5,4,9,6,7,8};

    // Call
    bool arrayIsSorted= checkIfArrayIsSortedAscending(unsortedArray, 8);

    //Assertion
    EXPECT_FALSE(arrayIsSorted);
}


TEST(PermutationSort, ShouldSortArray) {
    // Preparation
    int unsortedArray[] = {1,6,2,8,3,2,9,7};

    // Call
    permutationSort(unsortedArray,0,7);

    // Assertion
    // Verify that the array is sorted correctly
    for (size_t i = 1; i < 8; ++i) {
        EXPECT_LE(unsortedArray[i - 1], unsortedArray[i]);
    }
}

