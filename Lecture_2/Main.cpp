
#include<iostream>
#include "ArraySequence.h"
#include "StaticArray.h"
using namespace AlgorithmPractice;

int main(){
    const int arrayLength=5;
    StaticArray<int> numbersArray(arrayLength);
    int arrayTest[arrayLength];
    for(int i=0;i<arrayLength;i++){
        numbersArray.setAt(i,i+2);
        arrayTest[i]=i+2;
    }
    for(int i=0;i<arrayLength;i++){
        std::cout<<numbersArray.getAt(i)<<" ";
    }
    std::cout<<"\n";

    ArraySequence<int> dynamicArraySequence;
    dynamicArraySequence.build(arrayTest,arrayLength);
    dynamicArraySequence.printElements();
    dynamicArraySequence.insertFirst(30);
    dynamicArraySequence.insertLast(90);
    dynamicArraySequence.printElements();
}