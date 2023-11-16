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

bool permutationSort(int* array, int leftIndex, int rightIndex){
    if(leftIndex==rightIndex){
        if(checkIfArrayIsSortedAscending(array,rightIndex+1)){
            printArray(array,rightIndex+1);
            return false;
        }
        return true;
    }
    bool keepRecursive= true;
    for(int i=leftIndex;i<=rightIndex;i++){
        swapArrayElement(array,leftIndex,i);
        keepRecursive = permutationSort(array,leftIndex+1,rightIndex);
        if (!keepRecursive) {
            break;  // Stop further recursion
        }
        swapArrayElement(array,leftIndex,i);
    }
    return keepRecursive;
}

int main(){
    int arr[3]={3,1,2};
    int secondArr[5]={9,10,6,4,2};
    permutationSort(arr,0,2);
    permutationSort(secondArr,0,4);
    printArray(arr,3);
    printArray(secondArr,5);

  
}