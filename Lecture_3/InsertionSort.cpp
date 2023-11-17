#include<iostream>
#include "Utils.h"
using namespace AlgorithmPractice;

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

int main(){
    int arrayTest[4]={0,4,2,1};
    int secondArrayTest[5]={0,9,7,8,2};
    insertionSort(arrayTest,4);
    insertionSort(secondArrayTest,5);
    printArray(arrayTest,4);
    printArray(secondArrayTest,5);
}