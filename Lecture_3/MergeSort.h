#pragma once

namespace AlgorithmPractice{

template<typename ArrayType>
void copySliceOfArray(ArrayType *originalArray, ArrayType *newArray, int oriFirstIndex, int oriLastIndex, int newArrayFirstIndex=0){
    int newArraySize= oriLastIndex-oriFirstIndex+1;
    for (int i = 0; i < newArraySize; i++)
    {
        newArray[newArrayFirstIndex+i]=originalArray[oriFirstIndex+i];
    }
}

template<typename ArrayType>
void numericalMerge(ArrayType *array,int leftArrayFirstIndex, int leftArrayLastIndex, int rightArrayFirstIndex, int rightArrayLastIndex){
    int indexIter=leftArrayFirstIndex;
    int leftArrayIndexIter=0;
    int rightArrayIndexIter=0;
    
    int leftArraySize = leftArrayLastIndex-leftArrayFirstIndex+1;
    int rightArraySize = rightArrayLastIndex-rightArrayFirstIndex+1;
    
    ArrayType* leftArray= new ArrayType[leftArraySize];
    ArrayType* rightArray = new ArrayType[rightArraySize];
    copySliceOfArray(array,leftArray,leftArrayFirstIndex,leftArrayLastIndex);
    copySliceOfArray(array,rightArray,rightArrayFirstIndex,rightArrayLastIndex);
    
    while (leftArrayIndexIter < leftArraySize && rightArrayIndexIter < rightArraySize)
    {
        if(leftArray[leftArrayIndexIter] < rightArray[rightArrayIndexIter]){
            array[indexIter]=leftArray[leftArrayIndexIter];
            leftArrayIndexIter++;
        }else{
            array[indexIter]=rightArray[rightArrayIndexIter];
            rightArrayIndexIter++;
        }
        indexIter++;
    }

    copySliceOfArray(leftArray,array,leftArrayIndexIter,leftArraySize-1,indexIter);
    copySliceOfArray(rightArray,array,rightArrayIndexIter,rightArraySize-1,indexIter);
    delete[] leftArray;
    delete[] rightArray;
    
}

template<typename ArrayType>
void numericalMergeSort(ArrayType *array, int leftIndex, int rightIndex) {
    if (leftIndex >= rightIndex) {
        return;
    }
    int midIndex = (leftIndex + rightIndex) / 2;
    numericalMergeSort(array, leftIndex, midIndex);
    numericalMergeSort(array, midIndex + 1, rightIndex);
    numericalMerge(array, leftIndex, midIndex, midIndex + 1, rightIndex);
}

}
