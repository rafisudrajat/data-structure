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

TEST(CopySliceOfArray, ShouldCopySliceOfArrayInplace){
    //Prep
    int originalArray[]={1,2,3,4,5};
    int newArray[]={9,8,7,6,10};

    //Call
    copySliceOfArray(originalArray,newArray,1,3,2);

    //Assertion
    int expectedNewArray[]={9,8,2,3,4};
    for (size_t i = 1; i < 5; ++i) {
        EXPECT_TRUE(newArray[i]==expectedNewArray[i]);
    }
}

TEST(CopySliceOfVector, ShouldCopySliceOfVectorInplace){
    //Prep
    std::vector<int> originalVector={1,2,3,4,5};
    std::vector<int> newVector={9,8,7,6,10};

    //Call
    copySliceOfVector<int>(originalVector,newVector,1,3,2);

    //Assertion
    std::vector<int> expectedNewVector={9,8,2,3,4};
    for (size_t i = 1; i < 5; ++i) {
        EXPECT_TRUE(newVector[i]==expectedNewVector[i]);
    }
}