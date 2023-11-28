// A dumb algorithm that try to check if there is a sorted array in every permutation of an array
// it is dumb, but let's just make it anyway to give an example
#include<iostream>
#include "Utils.h"
#include "PermutationSort.h"

namespace AlgorithmPractice{
bool checkIfArrayIsSortedAscending(int* array,int size){
    for(int i=1;i<size;i++){
        if(array[i]<array[i-1]){
            return false;
        }
    }
    return true;
}

bool permutationSort(int* array, int leftIndex, int rightIndex){
    if(leftIndex==rightIndex){
        if(checkIfArrayIsSortedAscending(array,rightIndex+1)){
            return false;
        }
        return true;
    }
    bool keepRecursive= true;
    for(int i=leftIndex;i<=rightIndex;i++){
        swapArrayElement(array,leftIndex,i);
        keepRecursive = permutationSort(array,leftIndex+1,rightIndex);
        if (!keepRecursive) {
            break;  // Stop further recursion
        }
        swapArrayElement(array,leftIndex,i);
    }
    return keepRecursive;
}

}

