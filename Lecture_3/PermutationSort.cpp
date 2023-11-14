// A dumb algorithm that try to check if there is a sorted array in every permutation of an array
// it is dumb, but let's just make it anyway to give an example
#include<iostream>

void printArray(int* array,int size){
    for(int i=0;i<size;i++){
        std::cout<<array[i]<<" ";
    }
    std::cout<<"\n";
}

bool checkIfArrayIsSortedAscending(int* array,int size){
    for(int i=1;i<size;i++){
        if(array[i]<array[i-1]){
            return false;
        }
    }
    return true;
}

void swapArrayElement(int* array,int leftIndex, int rightIndex){
    if(leftIndex!=rightIndex){
        array[leftIndex] += array[rightIndex];
        array[rightIndex] = array[leftIndex] - array[rightIndex];
        array[leftIndex] -= array[rightIndex];
    }
}

void permutationSort(int* array, int leftIndex, int rightIndex, bool keepRecursive=true){
    if(!keepRecursive){
        return;
    }
    if(leftIndex==rightIndex){
        if(checkIfArrayIsSortedAscending(array,rightIndex+1)){
            printArray(array,rightIndex+1);
            keepRecursive=false;
        }
        return;
    }
    for(int i=leftIndex;i<=rightIndex;i++){
        swapArrayElement(array,leftIndex,i);
        permutationSort(array,leftIndex+1,rightIndex,keepRecursive);
        swapArrayElement(array,leftIndex,i);
    }
}

int main(){
    int arr[3]={1,2,3};
    permutationSort(arr,0,2);
    printArray(arr,3);
  
}