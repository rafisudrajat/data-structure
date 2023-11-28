#pragma once
#include <vector>
namespace AlgorithmPractice{
    void printArray(int* array,int size);

    void swapArrayElement(int* array,int leftIndex, int rightIndex);

    template<typename ElementType> 
    bool binarySearch(std::vector<ElementType>& vector, ElementType element){
        int leftIndex=0;
        int rightIndex=vector.size()-1;
           
        while (leftIndex<=rightIndex)
        {
            int midIndex=leftIndex+(rightIndex-leftIndex)/2;
            if(vector[midIndex]==element){
                return true;
            }

            if(vector[midIndex]<element){
                leftIndex=midIndex+1;
            }else{
                rightIndex=midIndex-1;
            }
        }

        return false;
    }

}