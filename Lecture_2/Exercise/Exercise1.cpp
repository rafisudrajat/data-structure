/*
Suppose the next pointer of the last node of a linked list points to an earlier node in the list,
creating a cycle. Given a pointer to the head of the list (without knowing its size), describe a
linear-time algorithm to find the number of nodes in the cycle. Can you do this while using
only constant additional space outside of the original linked list? 
*/

#include "LinkedListSequence.h"
#include "LinkedListNode.h"
using namespace AlgorithmPractice;

LinkedListNode<int>* builtCircularLinkedList(int* array, int length){
    LinkedListSequence<int>* linkedListSequence=new LinkedListSequence<int>();
    linkedListSequence->build(array,length);
    LinkedListNode<int>* circularLinkedListHead = linkedListSequence->getHead();
    LinkedListNode<int>* dummyPointer= circularLinkedListHead;
    while (dummyPointer->nextNode!=nullptr)
    {
        dummyPointer=dummyPointer->nextNode;
    }
    dummyPointer->nextNode=circularLinkedListHead;
    
    return circularLinkedListHead;
}

int getNumberOfNodesInCircularLinkedList(LinkedListNode<int>* circularLinkedList){
    LinkedListNode<int> *fastPointer= circularLinkedList, *slowPointer =circularLinkedList;
    fastPointer=fastPointer->nextNode->nextNode;
    slowPointer=slowPointer->nextNode;
    int counter=1;
    while (slowPointer!=fastPointer)
    {
        slowPointer=slowPointer->nextNode;
        fastPointer=fastPointer->nextNode->nextNode;
        counter++;
    }
    return counter;
}


int main(){
    int arrayTest[5]={1,2,4,6,7};
    int arrayTest2[6]={1,2,4,6,7,9};
    LinkedListNode<int>* circularLinkedList = builtCircularLinkedList(arrayTest,5);
    LinkedListNode<int>* circularLinkedList2 = builtCircularLinkedList(arrayTest2,6);
    std::cout<<getNumberOfNodesInCircularLinkedList(circularLinkedList)<<" ";
    std::cout<<getNumberOfNodesInCircularLinkedList(circularLinkedList2)<<" ";
    circularLinkedList->breakCircularLinkedList();
    circularLinkedList2->breakCircularLinkedList();
    delete circularLinkedList;
    delete circularLinkedList2;
}