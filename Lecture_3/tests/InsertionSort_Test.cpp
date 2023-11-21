#include "InsertionSort.h"
#include <gtest/gtest.h>
using namespace AlgorithmPractice;

// Test case to check if insertion sort works correctly
TEST(InsertionSortTest, SortsArray) {
    // Arrange
    int unsortedArray[] = {5, 3, 8, 1, 2};

    // Act
    insertionSort(unsortedArray, 5);

    // Assert
    // Verify that the array is sorted correctly
    for (size_t i = 1; i < 5; ++i) {
        EXPECT_LE(unsortedArray[i - 1], unsortedArray[i]);
    }
}