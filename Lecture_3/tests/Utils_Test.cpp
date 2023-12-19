#include "Utils.h"
#include <gtest/gtest.h>
#include <vector>
#include <unordered_map>
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

    BinarySearchReturnType<int> searchResult= binarySearch(testVector,element);

    EXPECT_TRUE(searchResult.ElementExist && searchResult.ElementValue==element);
    
}

TEST_P(BinarySearchTestNotFoundElement, ShouldReturnFalseIfElementNotExist){
    int element=GetParam();
    std::vector<int> testVector={1,5,8,9,10,12};

    BinarySearchReturnType<int> searchResult= binarySearch(testVector,element);

    EXPECT_FALSE(searchResult.ElementExist || searchResult.ElementValue==element);
  
}

void buildElementIndexPair(std::unordered_map<int,int>& hashMap, std::vector<int> vector){
    for(int i=0;i<vector.size();i++){
        hashMap.insert({vector[i],i});
    }
}

TEST_P(BinarySearchTestFoundElement, ShouldReturnValidIndexIfElementExist){
    int element=GetParam();
    std::vector<int> testVector={1,5,8,9,10,12};
    std::unordered_map<int,int> hashMap;
    buildElementIndexPair(hashMap,testVector);

    int indexSearchResult= binarySearchIndex(testVector,element);

    EXPECT_TRUE(indexSearchResult==hashMap[element]);
}

TEST_P(BinarySearchTestNotFoundElement, ShouldReturnInvalidIndexIfElementExist){
    int element=GetParam();
    std::vector<int> testVector={1,5,8,9,10,12};

    int indexSearchResult= binarySearchIndex(testVector,element);

    EXPECT_TRUE(indexSearchResult==-1);
}