#include "Utils.h"
#include <gtest/gtest.h>
#include <vector>
using namespace AlgorithmPractice;

// Define a parameterized test fixture
class BinarySearchTestFoundElement : public testing::TestWithParam<int> {};
class BinarySearchTestNotFoundElement : public testing::TestWithParam<int> {};

// Define the parameters for the test
INSTANTIATE_TEST_SUITE_P(ExistingElementToSearch,
    BinarySearchTestFoundElement,
    testing::Values(
        1,5,8,9,10,12
    )
);

INSTANTIATE_TEST_SUITE_P(NotExistingElementToSearch,
    BinarySearchTestNotFoundElement,
    testing::Values(
        19,22,19,69,20
    )
);

TEST_P(BinarySearchTestFoundElement, ShouldReturnTruetIfElementExist) {
    int element=GetParam();
    std::vector<int> testVector={1,5,8,9,10,12};

    bool searchResult= binarySearch(testVector,element);

    EXPECT_TRUE(searchResult);
    
}

TEST_P(BinarySearchTestNotFoundElement, ShouldReturnFalseIfElementNotExist){
    int element=GetParam();
    std::vector<int> testVector={1,5,8,9,10,12};

    bool searchResult= binarySearch(testVector,element);

    EXPECT_FALSE(searchResult);
  
}