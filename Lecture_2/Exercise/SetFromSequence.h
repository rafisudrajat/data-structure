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
        for(int i=0;i<this->length;i++){
            if(this->arrayContainer[i].key==key) return this->arrayContainer[i].value;
        }

        std::cout<<"Element not found"<<"\n";
        return ValueType();
    }

    void insertElement(Pair<KeyType,ValueType> pairElement){
        for(int i=0;i<this->length;i++){
            if(this->arrayContainer[i].key==pairElement.key) {
                this->setAtIndex(i,pairElement);
                return;
            }
        }
        this->insertLast(pairElement);
    }

    void deleteElement(KeyType key){
        for(int i=0;i<this->length;i++){
            if(this->arrayContainer[i].key==key) {
                this->deleteAtIndex(i);
                return;
            }
        }
    }

    void printSetElements(){
        for(int i=0;i<this->length;i++){
            std::cout<<this->arrayContainer[i].key<<", "<<this->arrayContainer[i].value<<"\n";
        }
    }

    int getLength(){
        return AmmortizedDynamicArray<Pair<KeyType,ValueType>>::getLength();
    }

    Pair<KeyType,ValueType> findMax(){
        Pair<KeyType,ValueType> maxPair=this->getAtIndex(0);
        for(int i=1;i<this->length;i++){
            if(maxPair.key<this->getAtIndex(i).key){
                maxPair=this->getAtIndex(i);
            }
        }
        return maxPair;
    }

    Pair<KeyType,ValueType> findMin(){
        Pair<KeyType,ValueType> maxPair=this->getAtIndex(0);
        for(int i=1;i<this->length;i++){
            if(maxPair.key>this->getAtIndex(i).key){
                maxPair=this->getAtIndex(i);
            }
        }
        return maxPair;
    }


    ~SetFromSequence(){

    }

};
}
