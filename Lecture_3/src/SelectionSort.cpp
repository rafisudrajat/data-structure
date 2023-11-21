#include <iostream>
#include "SelectionSort.h"
#include "Utils.h"
namespace AlgorithmPractice{

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

}


// using namespace AlgorithmPractice;
// int main(){
//     int arr[3]={3,1,2};
//     int secondArr[5]={9,10,6,4,2};
//     selectionSortIterative(arr,3);
//     selectionSortRecursive(secondArr,4);
//     printArray(arr,3);
//     printArray(secondArr,5);
  
// }