#include <iostream>
#include "MergeSort.h"
#include "Utils.h"
using namespace AlgorithmPractice;

int main(){
    int array[5]={3,2,5,1,4};
    numericalMergeSort(array,0,4);
    printArray(array,5);


    int arrayTest2[9]={1,5,2,4,7,1,2,9,10};
    numericalMergeSort(arrayTest2,0,8);
    printArray(arrayTest2,9);

}