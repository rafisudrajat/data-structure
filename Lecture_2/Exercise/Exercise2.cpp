/*
Given a data structure implementing the Sequence interface, show how to use it to implement
the Set interface. (Your implementation does not need to be efficient.)
*/
#include "SetFromSequence.h"
#include "Pair.h"
using namespace AlgorithmPractice;

int main(){
    Pair<int,int>* pairArray=new Pair<int,int>[5];
    for(int i=0;i<5;i++){
        pairArray[i]={i,i+7};
    }
    SetFromSequence<int,int> setDataStructure= SetFromSequence<int,int>(pairArray,5);
    setDataStructure.printSetElements();
    Pair<int,int> maxPair= setDataStructure.findMax();
    std::cout<<maxPair.key<<", "<<maxPair.value<<"\n";
    setDataStructure.deleteElement(0);
    setDataStructure.deleteElement(1);
    setDataStructure.insertElement({5,12});
    setDataStructure.printSetElements();
    std::cout<<setDataStructure.findElement(99)<<"\n";

}