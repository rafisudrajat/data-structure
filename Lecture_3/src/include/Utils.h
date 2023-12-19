#pragma once
#include <vector>
namespace AlgorithmPractice{

    template<typename ElementType>
    struct BinarySearchReturnType
    {
        bool ElementExist;
        ElementType ElementValue;
    };
    
    void printArray(int* array,int size);

    void swapArrayElement(int* array,int leftIndex, int rightIndex);

    template<typename ElementType> 
    BinarySearchReturnType<ElementType> binarySearch(std::vector<ElementType>& vector, ElementType element){
        int leftIndex=0;
        int rightIndex=vector.size()-1;
           
        while (leftIndex<=rightIndex)
        {
            int midIndex=leftIndex+(rightIndex-leftIndex)/2;
            if(vector[midIndex]==element){
                return {true,element};
            }

            if(vector[midIndex]<element){
                leftIndex=midIndex+1;
            }else{
                rightIndex=midIndex-1;
            }
        }

        return {false,ElementType()};
    }

    template<typename ElementType> 
    int binarySearchIndex(std::vector<ElementType>& vector, ElementType element){
        int leftIndex=0;
        int rightIndex=vector.size()-1;
           
        while (leftIndex<=rightIndex)
        {
            int midIndex=leftIndex+(rightIndex-leftIndex)/2;
            if(vector[midIndex]==element){
                return midIndex;
            }

            if(vector[midIndex]<element){
                leftIndex=midIndex+1;
            }else{
                rightIndex=midIndex-1;
            }
        }

        return -1;
    }

}