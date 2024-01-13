#pragma once
#include<iostream>

namespace AlgorithmPractice{
    template<typename ElementType>
    class AmmortizedDynamicArray
    {
    protected:
        int length;
        int capacity;
        ElementType* arrayContainer;
        int sizingRatio;
        int upperBound;
        int lowerBound;
    public:
        //TO DO: At getter method
        AmmortizedDynamicArray(int sizingRatio=2){
            this->sizingRatio=sizingRatio;
            arrayContainer=nullptr;
            length=0;
            capacity=0;
            calculateBound();
            resize(length);
        }

        ~AmmortizedDynamicArray(){
            delete[] arrayContainer;
        }

        void build(ElementType* iterableData, int iterableDataSize){
            for(int i=0;i<iterableDataSize;i++){
                insertLast(iterableData[i]);
            }
        }

        void printElements(){
            for(int i=0;i<length;i++){
                std::cout<<arrayContainer[i]<<" ";
            }
            std::cout<<"\n";
        }
        ElementType getAtIndex(int index){
            if(index>=length){
                throw "Index must be less than the number of elements";
            }
            return arrayContainer[index];
        }
        void setAtIndex(int index, ElementType value){
            if(index>=length){
                throw "Index must be less than the number of elements";
            }
            arrayContainer[index]=value;   
        }
        void insertAtIndex(int index, ElementType value){
            resize(length+1);
            copyArrayContainerToNewArrayContainerBackward(arrayContainer,index,length-1,1);
            arrayContainer[index]=value;
            length++;
        }

        void deleteAtIndex(int index){
            copyArrayContainerToNewArrayContainer(arrayContainer,index+1,length-1,-1);
            deleteLast();
        }
        
        void insertFirst(ElementType value){
            insertAtIndex(0,value);
        }

        void deleteFirst(){
            deleteAtIndex(0);
        }

        void insertLast(ElementType value){
            resize(length+1);
            arrayContainer[length]=value;
            length++;
        }

        void deleteLast(){
            length--;
            resize(length);
        }

        int getLength(){
            return length;
        }

    protected:
        void calculateBound(){
            upperBound=capacity;
            lowerBound=capacity/(sizingRatio*sizingRatio);
        }

        int max(int firstNumber, int secondNumber){
            return (firstNumber>secondNumber) ? firstNumber : secondNumber; 
        }

        void resize(int updatedLength){
            if(lowerBound<updatedLength && updatedLength<upperBound) return;
            int newArrayCapacity = max(1,updatedLength) * sizingRatio;
            ElementType* newArrayContainer = new ElementType[newArrayCapacity];
            copyArrayContainerToNewArrayContainer(newArrayContainer,0,length-1);
            
            delete[] arrayContainer;
            arrayContainer=newArrayContainer;
            capacity=newArrayCapacity;
            
            calculateBound();
        }

        void copyArrayContainerToNewArrayContainer(ElementType* newArrayContainer, int startIndex, int lastIndex, int offset=0){
            for(int i=startIndex;i<=lastIndex;i++){
                newArrayContainer[i+offset]=arrayContainer[i];
            }
        }

        void copyArrayContainerToNewArrayContainerBackward(ElementType* newArrayContainer, int startIndex, int lastIndex, int offset=0){
            for(int i=lastIndex;i>=startIndex;i--){
                newArrayContainer[i+offset]=arrayContainer[i];
            }
        }

    };
    
}