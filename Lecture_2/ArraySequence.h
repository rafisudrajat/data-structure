#include<iostream>

namespace AlgorithmPractice{

template<typename ElementType>
class ArraySequence
{
private:
    ElementType* elements;
    int size;

public:
    ArraySequence(){
        size=0;
        elements=new ElementType[size];
    }
    
    ~ArraySequence(){
        delete[] elements;
    }
    
    int getLength(){
        return size;
    }

    void printElements(){
        for(int index=0;index<size;index++){
            std::cout<<elements[index]<<" ";
        }
        std::cout<<"\n";
    }

    void build(ElementType* iterableData, int iterableDataSize){
        delete[] elements;
        elements = new ElementType[iterableDataSize];
        size=iterableDataSize;
        for(int index=0;index<iterableDataSize;index++){
            elements[index]=iterableData[index];
        }
    }

    ElementType getAt(int index){
        if(index>=size){
            throw "Index must be less than the number of elements";
        }
        return elements[index];
    }

    void setAt(int index, ElementType value){
        if(index>=size){
            throw "Index must be less than the number of elements";
        }
        elements[index]=value;   
    }
    
    void insertAtIndex(int index, ElementType value){
        if(index>=size+1){
            throw "Index must be less than the number of elements + 1";
        }
        ElementType* newElements= new ElementType[size+1];
        copyElementsToNewElementsPartially(newElements,0,index-1);
        newElements[index]=value;
        copyElementsToNewElementsPartially(newElements,index,size-1,index+1);
        size++;
        delete[] elements;
        elements=newElements;
    }

    void deleteAtIndex(int index){
        ElementType* newElements= new ElementType[size-1];
        copyElementsToNewElementsPartially(newElements,0,index-1);
        copyElementsToNewElementsPartially(newElements,index+1,size-1,index);
        size--;
        delete[] elements;
        elements=newElements;
    }

    void insertFirst(ElementType value){
        insertAtIndex(0,value);
    }

    void insertLast(ElementType value){
        insertAtIndex(size,value);
    }

    void deleteFirst(){
        deleteAtIndex(0);
    }

    void deleteLast(){
        deleteAtIndex(size);
    }

private:
    void copyElementsToNewElementsPartially(ElementType* newElements, int startIndex, int lastIndex, int newElementsStartIndex=0){
        for(int i=startIndex;i<=lastIndex;i++){
            newElements[newElementsStartIndex+i]=elements[i];
        }
    }

};

}