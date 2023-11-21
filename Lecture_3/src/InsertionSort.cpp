#include<iostream>
#include "InsertionSort.h"
namespace AlgorithmPractice{

void swapArrayElement(int* array,int leftIndex, int rightIndex){
    if(leftIndex!=rightIndex){
        array[leftIndex] += array[rightIndex];
        array[rightIndex] = array[leftIndex] - array[rightIndex];
        array[leftIndex] -= array[rightIndex];
    }
}

void sortSubArray(int *array, int currentIndex){
    int arrayElement=array[currentIndex];
    while (currentIndex>0 && array[currentIndex-1]>arrayElement)
    {
        swapArrayElement(array,currentIndex-1,currentIndex);
        currentIndex--;
    }   
}

void insertionSort(int* array,int arraySize){
    for(int i=0;i<arraySize;i++){
        sortSubArray(array,i);
    }
}

}