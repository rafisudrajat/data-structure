#include "Utils.h"
#include <iostream>
namespace AlgorithmPractice{

void printArray(int* array,int size){
    for(int i=0;i<size;i++){
        std::cout<<array[i]<<" ";
    }
    std::cout<<"\n";
}

void swapArrayElement(int* array,int leftIndex, int rightIndex){
    if(leftIndex!=rightIndex){
        array[leftIndex] += array[rightIndex];
        array[rightIndex] = array[leftIndex] - array[rightIndex];
        array[leftIndex] -= array[rightIndex];
    }
}

}