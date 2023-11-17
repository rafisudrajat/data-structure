#pragma once
#include <iostream>
namespace AlgorithmPractice{
    void printArray(int* array,int size){
        for(int i=0;i<size;i++){
            std::cout<<array[i]<<" ";
        }
        std::cout<<"\n";
    }

}