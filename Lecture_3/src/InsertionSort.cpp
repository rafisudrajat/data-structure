#include<iostream>
#include "InsertionSort.h"
#include "Utils.h"
namespace AlgorithmPractice{

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