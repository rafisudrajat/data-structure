#include<iostream>

namespace AlgorithmPractice{

template<typename ElementType>
class ArraySequence
{
private:
    ElementType* elements;
    int length;

public:
    ArraySequence(){
        length=0;
        elements=new ElementType[length];
    }
    
    ~ArraySequence(){
        delete[] elements;
    }
    
    int getLength(){
        return length;
    }

    void printElements(){
        for(int index=0;index<length;index++){
            std::cout<<elements[index]<<" ";
        }
        std::cout<<"\n";
    }

    void build(ElementType* iterableData, int iterableDataSize){
        delete[] elements;
        elements = new ElementType[iterableDataSize];
        length=iterableDataSize;
        for(int index=0;index<iterableDataSize;index++){
            elements[index]=iterableData[index];
        }
    }

    ElementType getAtIndex(int index){
        if(index>=length){
            throw "Index must be less than the number of elements";
        }
        return elements[index];
    }

    void setAtIndex(int index, ElementType value){
        if(index>=length){
            throw "Index must be less than the number of elements";
        }
        elements[index]=value;   
    }
    
    void insertAtIndex(int index, ElementType value){
        if(index>=length+1){
            throw "Index must be less than the number of elements + 1";
        }
        ElementType* newElements= new ElementType[length+1];
        copyElementsToNewElementsPartially(newElements,0,index-1);
        newElements[index]=value;
        copyElementsToNewElementsPartially(newElements,index,length-1,1);
        length++;
        delete[] elements;
        elements=newElements;
    }

    void deleteAtIndex(int index){
        if(index>=length){
            throw "Index must be less than the number of elements";
        }
        ElementType* newElements= new ElementType[length-1];
        copyElementsToNewElementsPartially(newElements,0,index-1);
        copyElementsToNewElementsPartially(newElements,index+1,length-1,index);
        length--;
        delete[] elements;
        elements=newElements;
    }

    void insertFirst(ElementType value){
        insertAtIndex(0,value);
    }

    void deleteFirst(){
        deleteAtIndex(0);
    }

    void insertLast(ElementType value){
        insertAtIndex(length,value);
    }

    void deleteLast(){
        deleteAtIndex(length-1);
    }

private:
    void copyElementsToNewElementsPartially(ElementType* newElements, int startIndex, int lastIndex, int startIndexOffset=0){
        for(int i=startIndex;i<=lastIndex;i++){
            newElements[i+startIndexOffset]=elements[i];
        }
    }

};

}