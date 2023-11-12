// The set must be dynamic
#pragma once
#include "../AmmortizedDynamicArray.h"
#include "Pair.h"
#include <iostream>
namespace AlgorithmPractice{

template <typename KeyType, typename ValueType>
class SetFromSequence: private AmmortizedDynamicArray<Pair<KeyType,ValueType>> {

public:
    SetFromSequence(Pair<KeyType,ValueType>* elementArray,int arraySize):AmmortizedDynamicArray<Pair<KeyType,ValueType>>(){
        for(int i=0;i<arraySize;i++){
            insertElement(elementArray[i]);
        }
    }

    ValueType findElement(KeyType key){
        for(int i=0;i<length;i++){
            if(arrayContainer[i].key==key) return arrayContainer[i].value;
        }

        std::cout<<"Element not found";
        return ValueType();
    }

    void insertElement(Pair<KeyType,ValueType> pairElement){
        for(int i=0;i<length;i++){
            if(arrayContainer[i].key==pairElement.key) {
                setAtIndex(i,pairElement);
                return;
            }
        }
        insertLast(pairElement);
    }

    void deleteElement(KeyType key){
        for(int i=0;i<length;i++){
            if(arrayContainer[i].key==key) {
                deleteAtIndex(i);
                return;
            }
        }
    }

    void printSetElements(){
        for(int i=0;i<length;i++){
            std::cout<<arrayContainer[i].key<<", "<<arrayContainer[i].value<<"\n";
        }
    }

    int getLength(){
        return AmmortizedDynamicArray<Pair<KeyType,ValueType>>::getLength();
    }

    Pair<KeyType,ValueType> findMax(){
        Pair<KeyType,ValueType> maxPair=getAtIndex(0);
        for(int i=1;i<length;i++){
            if(maxPair.key<getAtIndex(i).key){
                maxPair=getAtIndex(i);
            }
        }
        return maxPair;
    }

    Pair<KeyType,ValueType> findMin(){
        Pair<KeyType,ValueType> maxPair=getAtIndex(0);
        for(int i=1;i<length;i++){
            if(maxPair.key>getAtIndex(i).key){
                maxPair=getAtIndex(i);
            }
        }
        return maxPair;
    }


    ~SetFromSequence(){

    }

};
}
