#include "SortedArraySet.h"
#include <gtest/gtest.h>
#include <algorithm>
#include "MergeSort.h"
#include <unordered_map>

using namespace AlgorithmPractice;

void buildElementFrequencyPair(std::unordered_map<int,int>& hashMap, std::vector<int> vector){
    for(int i=0;i<vector.size();i++){
        hashMap[vector[i]]++;
    }
}

// Test case to check if insertion sort works correctly
TEST(SortedArraySetTest, ShouldSortedArrayInConstructor) {
    // Preparation
    const int arraySize=9;
    int randomArray[arraySize]={9,5,1,2,10,7,6,4,8};
    
    // Call
    SortedArraySet<int> sortedArraySet(randomArray,arraySize);
     
    // Assertion
    // Verify that the array is sorted correctly
    int sortedArray[arraySize];
    std::copy(randomArray, randomArray + arraySize, sortedArray);
    numericalMergeSort(sortedArray,0,arraySize-1);
    for (int i = 1; i < arraySize; ++i) {
        EXPECT_EQ(sortedArraySet.findIndex(sortedArray[i]),i);
    }
}

TEST(SortedArraySetTest, ShouldNotDuplicateElementInConstructor){
    // Prep
    const int arraySize=9;
    int randomArray[arraySize]={1,1,1,1,9,9,6,1,1};

    // Call
    SortedArraySet<int> sortedArraySet(randomArray,arraySize);
     
    // Assertion
    std::unordered_map<int,int> hashMap;
    buildElementFrequencyPair(hashMap,sortedArraySet.returnAllElementAsVector());
    for(auto& elementFrequencyPair: hashMap){
        ASSERT_EQ(elementFrequencyPair.second,1);
    }

}

TEST(SortedArraySet_FindTest, ShoudlReturnTrueIfElementFound){
    // Prep
    const int arraySize=5;
    int arrayElements[arraySize]={8,9,2,4,1};
    SortedArraySet<int> sortedArraySet(arrayElements,arraySize);

    for(int i=0;i<arraySize;i++){
        // Call
        bool actualSearchResult=sortedArraySet.find(arrayElements[i]);
        
        // Assertion
        EXPECT_TRUE(actualSearchResult);
    }
}

TEST(SortedArraySet_FindTest,ShouldReturnFalseIfElementNotFound){
    // Prep
    const int arraySize=5;
    int arrayElements[arraySize]={8,9,2,4,1};
    SortedArraySet<int> sortedArraySet(arrayElements,arraySize);
    int arrayTest[arraySize]={10,20,3,89,5};

    for(int i=0;i<arraySize;i++){
        // Call
        bool actualSearchResult = sortedArraySet.find(arrayTest[i]);
        
        // Assertion
        EXPECT_FALSE(actualSearchResult);
    }
}

TEST(SortedArraySet_InsertTest, ShouldNotInsertDuplicatedElement){
    // Prep
    const int arraySize=8;
    int arrayElements[arraySize]={8,9,9,2,4,1,1,2};
    SortedArraySet<int> sortedArraySet;

    // Call
    for(int i=0;i<arraySize;i++){
        sortedArraySet.insert(arrayElements[i]);
    }

    //Assertion
    std::unordered_map<int,int> hashMap;
    buildElementFrequencyPair(hashMap,sortedArraySet.returnAllElementAsVector());
    for(auto& elementFrequencyPair:hashMap){
        ASSERT_EQ(elementFrequencyPair.second,1);
    }
}



TEST(SortedArraySet_FindIndexTest, ShouldReturnCorrectIndexWhenElemetExist){
    // Prep
    const int arraySize=7;
    int arrayElements[arraySize]={8,9,2,4,1,1,2};
    SortedArraySet<int> sortedArraySet(arrayElements,arraySize);
    
    // Call
    std::vector<int> searchResults;
    for(int i=0;i<arraySize;i++){
        searchResults.push_back(sortedArraySet.findIndex(arrayElements[i]));
    }

    // Assertion
    std::unordered_map<int,int> hashMap;
    //check if the index to an element is unique
    for(int i=0;i<arraySize;i++){
        if(hashMap.count(arrayElements[i])>0){
            ASSERT_EQ(searchResults[i],hashMap[arrayElements[i]]);
        }else{
            ASSERT_TRUE(searchResults[i]>=0 && searchResults[i]<arraySize);
        }
        hashMap.insert({arrayElements[i],searchResults[i]});
    }

}

TEST(SortedArraySet_FindIndexTest, ShouldReturnNegativeIndexWhenElemetNotExist){
    // Prep
    const int arraySize=7;
    int arrayElements[arraySize]={8,9,2,4,1,1,2};
    SortedArraySet<int> sortedArraySet(arrayElements,arraySize);
    
    // Call
    int searchResult=sortedArraySet.findIndex(19);

    // Assertion
    ASSERT_EQ(searchResult,-1);   
}

TEST(SortedArraySet_DeleteElement, ShouldNotContainDeletedElement){
    // Prep
    const int arraySize=7;
    int arrayElements[arraySize]={8,9,2,4,1,1,2};
    SortedArraySet<int> sortedArraySet(arrayElements,arraySize);

    // Call
    sortedArraySet.deleteElement(1);

    // Assertion
    ASSERT_FALSE(sortedArraySet.find(1));
}

TEST(SortedArraySet_DeleteElement, ShouldElementsNumberShouldReducedBy1){
    // Prep
    const int arraySize=7;
    int arrayElements[arraySize]={8,9,2,4,1,1,2};
    SortedArraySet<int> sortedArraySet(arrayElements,arraySize);
    int numberOfELementsBeforeDeletion= sortedArraySet.returnAllElementAsVector().size();

    // Call
    sortedArraySet.deleteElement(1);

    // Assertion
    int numberOfELementsAfterDeletion= sortedArraySet.returnAllElementAsVector().size();
    ASSERT_TRUE(numberOfELementsAfterDeletion == numberOfELementsBeforeDeletion-1);
}

TEST(SortedArraySet_FindMaxTest, ShouldReturnMaxElement){
    // Prep
    const int arraySize=7;
    int arrayElements[arraySize]={8,9,2,4,1,1,2};
    SortedArraySet<int> sortedArraySet(arrayElements,arraySize);

    // Call
    int maxElement=sortedArraySet.findMax();

    // Assertion
    ASSERT_EQ(maxElement,9);
}

TEST(SortedArraySet_FindMinTest, ShouldReturnMinElement){
    // Prep
    const int arraySize=7;
    int arrayElements[arraySize]={8,9,2,4,1,1,2};
    SortedArraySet<int> sortedArraySet(arrayElements,arraySize);

    // Call
    int minElement=sortedArraySet.findMin();

    // Assertion
    ASSERT_EQ(minElement,1);
}

TEST(SortedArraySet_FindNextTest, ShouldReturnNearestGreaterElement){
    // Prep
    const int arraySize=7;
    int arrayElements[arraySize]={8,9,2,4,1,1,2};
    SortedArraySet<int> sortedArraySet(arrayElements,arraySize);

    // Call
    int findNextResult = sortedArraySet.findNext(8);

    // Assertion
    ASSERT_EQ(findNextResult,9);
}

TEST(SortedArraySet_FindPrevTest, ShouldReturnNearestSmallerElement){
    // Prep
    const int arraySize=7;
    int arrayElements[arraySize]={8,9,2,4,1,1,2};
    SortedArraySet<int> sortedArraySet(arrayElements,arraySize);

    // Call
    int findPrevResult = sortedArraySet.findPrev(8);

    // Assertion
    ASSERT_EQ(findPrevResult,4);
}





