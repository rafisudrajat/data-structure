#pragma once
#include <vector>

namespace AlgorithmPractice{

template<typename ArrayType>
void copySliceOfArray(ArrayType *originalArray, ArrayType *newArray, int oriFirstIndex, int oriLastIndex, int newArrayFirstIndex=0){
    int arraySize= oriLastIndex-oriFirstIndex+1;
    for (int i = 0; i < arraySize; i++)
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

template<typename VectorType>
void copySliceOfVector(std::vector<VectorType>& originalVector, std::vector<VectorType>& newVector, int oriFirstIndex, int oriLastIndex, int newVectorFirstIndex=0){
    int vectorSize= oriLastIndex-oriFirstIndex+1;
    for (int i = 0; i < vectorSize; i++)
    {
        newVector[newVectorFirstIndex+i]=originalVector[oriFirstIndex+i];
    }
}

template<typename VectorType>
void numericalMerge(std::vector<VectorType> &vector, int leftVectorFirstIndex, int leftVectorLastIndex, int rightVectorFirstIndex, int rightVectorLastIndex){
    int indexIter=leftVectorFirstIndex;
    int leftVectorIndexIter=0;
    int rigthVectorIndexIter=0;

    int leftVectorSize = leftVectorLastIndex-leftVectorFirstIndex+1;
    int rightVectorSize = rightVectorLastIndex-rightVectorFirstIndex+1;

    std::vector<VectorType> leftVector(leftVectorSize);
    std::vector<VectorType> rightVector(rightVectorSize);

    copySliceOfVector(vector,leftVector,leftVectorFirstIndex,leftVectorLastIndex);
    copySliceOfVector(vector,rightVector,rightVectorFirstIndex,rightVectorLastIndex);

    while (leftVectorIndexIter < leftVectorSize && rigthVectorIndexIter < rightVectorSize)
    {
        if(leftVector[leftVectorIndexIter] < rightVector[rigthVectorIndexIter]){
            vector[indexIter]=leftVector[leftVectorIndexIter];
            leftVectorIndexIter++;
        }else{
            vector[indexIter]=rightVector[rigthVectorIndexIter];
            rigthVectorIndexIter++;
        }
        indexIter++;
    }


    copySliceOfVector(leftVector,vector,leftVectorIndexIter,leftVectorSize-1,indexIter);
    copySliceOfVector(rightVector,vector,rigthVectorIndexIter,rightVectorSize-1,indexIter);
   
}

template<typename VectorType>
void numericalMergeSort(std::vector<VectorType> &vector, int leftIndex, int rightIndex){
    if (leftIndex >= rightIndex) {
        return;
    }
    int midIndex = (leftIndex + rightIndex) / 2;
    numericalMergeSort(vector, leftIndex, midIndex);
    numericalMergeSort(vector, midIndex + 1, rightIndex);
    numericalMerge(vector, leftIndex, midIndex, midIndex + 1, rightIndex);
}



}
