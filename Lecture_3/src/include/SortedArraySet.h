#pragma once
#include <vector>
#include "MergeSort.h"
#include "Utils.h"
#include <iostream>

namespace AlgorithmPractice{
template<typename ElementType>
class SortedArraySet
{
//build -> depends on the sorting algorithm, in this implementation we will use merge sort -> O(n log n)
//find -> O(log n)
//insert -> O(n)
//delete -> O(n)
//findmin -> O(1)
//findmax -> O(1)
//findnext -> O(log n)
//findprev -> O(log n)
private:
    std::vector<ElementType> setContainer;
public:
    SortedArraySet(ElementType* sequence, int sequenceSize){
        if(sequenceSize==0){
            return;
        }
        numericalMergeSort(sequence,0,sequenceSize-1);
        int lastSequenceElement=sequence[0];
        setContainer.push_back(lastSequenceElement);
        for(int i=1; i<sequenceSize; i++){
            if(sequence[i]!=lastSequenceElement){
                lastSequenceElement=sequence[i];
                setContainer.push_back(lastSequenceElement);
            }
        }
    }

    SortedArraySet(){

    }

    ~SortedArraySet(){

    }


    bool find(ElementType element){
        BinarySearchReturnType<ElementType> searchResult= binarySearch(setContainer, element);
        return searchResult.ElementExist;   
    }

    int findIndex(ElementType element){
        return binarySearchIndex(setContainer,element);
    }

    void insert(ElementType element){
        int index = binarySearchIndex(setContainer,element);
        if(index != -1){
            return;
        }
        
        for (index = 0; index < setContainer.size(); index++)
        {
            if(setContainer[index]>element){
                break;
            }
        }
        
        setContainer.insert(setContainer.begin()+index,element);
    }

    void deleteElement(ElementType element){
        int index = binarySearchIndex(setContainer,element);
        setContainer.erase(setContainer.begin()+index);
    }

    ElementType findMin(){
        return *setContainer.begin();
    }

    ElementType findMax(){
        return *(setContainer.end()-1);
    }

    ElementType findNext(ElementType element){
        int index = binarySearchIndex(setContainer,element);
        if(index<0){
            std::cout<< "Cannot find index"<<'\n';
            return ElementType();
        }
        return setContainer[index+1];
    }

    ElementType findPrev(ElementType element){
        int index = binarySearchIndex(setContainer,element);
        if(index<0){
            std::cout<< "Cannot find index"<<'\n';
            return ElementType();
        }
        return setContainer[index-1];   
    }

    std::vector<ElementType> returnAllElementAsVector(){
        return setContainer;
    }
};





}


