#pragma once
#include <vector>
#include "MergeSort.h"
//build -> depends on the sorting algorithm, in this implementation we will use merge sort -> O(n log n)
//find -> O(log n)
//insert -> O(n)
//delete -> O(n)
//findmin -> O(1)
//findmax -> O(1)
//findnext -> O(log n)
//findprev -> O(log n)

namespace AlgorithmPractice{
template<typename ElementType>
class SortedArraySet
{
private:
    std::vector<ElementType> setContainer;
public:
    SortedArraySet(ElementType* sequence, int sequenceSize){
        setContainer.resize(sequenceSize);
        for(int i=0; i<sequenceSize; i++){
            setContainer.push_back(sequence[i]);
        }

        numericalMergeSort(setContainer,0,sequenceSize-1);
    }

    ~SortedArraySet(){

    }


    bool find(ElementType element){
        
    }

};





}


