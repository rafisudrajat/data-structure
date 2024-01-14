#pragma once
#include "LinkedListSequence.h"
using namespace AlgorithmPractice;
namespace TestUtils{
    LinkedListSequence<int> buildLinkedListSequence(int* arrayForTest){
        LinkedListSequence<int> linkedListSequence;
        linkedListSequence.build(arrayForTest,5);

        return linkedListSequence;
    }
}