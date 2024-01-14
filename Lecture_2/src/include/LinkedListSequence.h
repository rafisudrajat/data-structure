#include "LinkedListNode.h"
#include <iostream>
#pragma once
namespace AlgorithmPractice{
template<typename NodeValueType>
class LinkedListSequence
{
private:
    LinkedListNode<NodeValueType>* head;
    int length;
public:
    LinkedListSequence(){
        length=0;
        head=nullptr;
    }
    ~LinkedListSequence(){

    }

    LinkedListNode<NodeValueType>* getHead(){
        return head;
    }
    
    void build(NodeValueType* iterableData, int iterableDataSize){
        for(int index=iterableDataSize-1;index>=0;index--){
            insertFirst(iterableData[index]);
        }
    }

    void printElements(){
        LinkedListNode<NodeValueType>* nodePointer=head;
        std::cout<<nodePointer->value<<" ";
        for(int index=1;index<length;index++){
            nodePointer=nodePointer->nextNode;
            std::cout<<nodePointer->value<<" ";
        }
        std::cout<<"\n";
    }

    NodeValueType getAtIndex(int index){
        if(index>=length){
            throw "Index must be less than the number of elements";
        }
        LinkedListNode<NodeValueType>* nodePointer=head;
        for(int idx=1;idx<=index;idx++){
            nodePointer=nodePointer->nextNode;
        }
        return nodePointer->value;
    }

    void setAtIndex(int index, NodeValueType nodeValue){
        if(index>=length){
            throw "Index must be less than the number of elements";
        }
        LinkedListNode<NodeValueType>* nodePointer=head;
        for(int idx=1;idx<=index;idx++){
            nodePointer=nodePointer->nextNode;
        }
        nodePointer->value=nodeValue;
    }

    void insertFirst(NodeValueType nodeValue){
        LinkedListNode<NodeValueType>* newHead=new LinkedListNode<NodeValueType>(nodeValue,head);
        head=newHead;
        length++;
    }

    void deleteFirst(){
        LinkedListNode<NodeValueType>* headPointer=head;
        head=head->nextNode;
        headPointer->nextNode=nullptr;
        delete headPointer;
        length--;
    }

    void insertAtIndex(int index, NodeValueType nodeValue){
        if(index>=length+1){
            throw "Index must be less than the number of elements";
        }
        LinkedListNode<NodeValueType>* nodePointer=head;
        for(int idx=1;idx<index;idx++){
            nodePointer=nodePointer->nextNode;
        }
        LinkedListNode<NodeValueType>* newInsertedNode= new LinkedListNode<NodeValueType>(nodeValue,nodePointer->nextNode);
        nodePointer->nextNode=newInsertedNode;
        length++;
    }

    void deleteAtIndex(int index){
        if(index>=length){
            throw "Index must be less than the number of elements";
        }
        LinkedListNode<NodeValueType>* nodePointer=head;
        for(int idx=1;idx<index;idx++){
            nodePointer=nodePointer->nextNode;
        }
        LinkedListNode<NodeValueType>* nodePointerNext=nodePointer->nextNode;
        if(length!=1){
            nodePointer->nextNode=nodePointerNext->nextNode;
        }
        nodePointerNext->nextNode=nullptr;
        delete nodePointerNext;
        length--;
    }

    void insertLast(NodeValueType nodeValue){
        insertAtIndex(length,nodeValue);
    }

    void deleteLast(){
        deleteAtIndex(length-1);
    }
};

}
