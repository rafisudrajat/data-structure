#include<iostream>
#include "ArraySequence.h"
#include "StaticArray.h"
#include "LinkedListSequence.h"
using namespace AlgorithmPractice;

void ArraySequenceTest(){
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
    dynamicArraySequence.insertAtIndex(2,69);
    dynamicArraySequence.printElements();
    dynamicArraySequence.deleteLast();
    dynamicArraySequence.printElements();
}

void LinkedListSequenceTest(){
    const int arrayLength=5;
    int arrayTest[arrayLength];
    for(int i=0;i<arrayLength;i++){
        arrayTest[i]=i+2;
    }
    LinkedListSequence<int> linkedListSequence;
    linkedListSequence.build(arrayTest,arrayLength);
    linkedListSequence.printElements();
    linkedListSequence.insertFirst(30);
    linkedListSequence.insertLast(90);
    linkedListSequence.insertAtIndex(2,69);
    linkedListSequence.printElements();
    linkedListSequence.deleteLast();
    linkedListSequence.printElements();

}

int main(){
    ArraySequenceTest();
    std::cout<<"\n";
    LinkedListSequenceTest();
}