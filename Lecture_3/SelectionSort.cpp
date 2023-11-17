#include <iostream>
#include "Utils.h"
using namespace AlgorithmPractice;


int findMaxElementIndexBetweenLeftIndexAndRightIndex(int* array,int leftIndex, int rightIndex){
    int maxElement=array[leftIndex];
    int maxElementIndex=leftIndex;
    for(int i=leftIndex+1;i<=rightIndex;i++){
        if(maxElement<array[i]){
            maxElement=array[i];
            maxElementIndex=i;
        }
    }
    return maxElementIndex;
}
void swapArrayElement(int* array,int leftIndex, int rightIndex){
    if(leftIndex!=rightIndex){
        array[leftIndex] += array[rightIndex];
        array[rightIndex] = array[leftIndex] - array[rightIndex];
        array[leftIndex] -= array[rightIndex];
    }
}
void selectionSortIterative(int* array,int size){
    for(int i=size-1;i>=0;i--){
        int maxIndex=findMaxElementIndexBetweenLeftIndexAndRightIndex(array,0,i);
        swapArrayElement(array,i,maxIndex);
    }
}

void selectionSortRecursive(int* array, int lastIndex){
    if(lastIndex==0){
        return;
    }
    int maxIndex=findMaxElementIndexBetweenLeftIndexAndRightIndex(array,0,lastIndex);
    swapArrayElement(array,lastIndex,maxIndex);
    selectionSortIterative(array,lastIndex-1);
}


int main(){
    int arrayTest[4]={0,4,2,1};
    int secondArrayTest[5]={0,9,7,8,2};
    selectionSortIterative(arrayTest,4);
    selectionSortRecursive(secondArrayTest,4);
    printArray(arrayTest,4);
    printArray(secondArrayTest,5);

}
