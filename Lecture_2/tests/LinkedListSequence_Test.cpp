#include <gtest/gtest.h>
#include "LinkedListSequence.h"
#include "TestUtils.h"
using namespace AlgorithmPractice;

TEST(LinkedListSequence_BuildTest, ShouldBuildSequence) {
    int arrayForTest[]={1,2,3,4,5};
    LinkedListSequence<int> linkedListSequence;

    linkedListSequence.build(arrayForTest,5);

    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(linkedListSequence.getAtIndex(i),arrayForTest[i]);
    }
}

TEST(LinkedListSequence_SetAtIndexTest, ShouldSetCorrectValue){
    int arrayForTest[]={1,2,3,4,5};
    LinkedListSequence<int> linkedListSequence=TestUtils::buildLinkedListSequence(arrayForTest);

    linkedListSequence.setAtIndex(2,10);

    arrayForTest[2]=10;
    for (size_t i = 0; i < 5; i++)
    {
        ASSERT_EQ(linkedListSequence.getAtIndex(i),arrayForTest[i]);
    }
}

TEST(LinkedListSequence_InsertFirstTest, ShouldInsertAtFirstIndex){
    int arrayForTest[]={1,2,3,4,5};
    LinkedListSequence<int> linkedListSequence=TestUtils::buildLinkedListSequence(arrayForTest);

    linkedListSequence.insertFirst(10);

    ASSERT_EQ(linkedListSequence.getAtIndex(0),10);
    for (size_t i = 0; i < 5; i++)
    {
        ASSERT_EQ(linkedListSequence.getAtIndex(i+1),arrayForTest[i]);
    }
}

TEST(LinkedListSequence_DeleteFirstTest,ShouldDeleteFirstIndex){
    int arrayForTest[]={1,2,3,4,5};
    LinkedListSequence<int> linkedListSequence=TestUtils::buildLinkedListSequence(arrayForTest);

    linkedListSequence.deleteFirst();

    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(linkedListSequence.getAtIndex(i),arrayForTest[i+1]);
    }
}

TEST(LinkedListSequence_InsertAtIndexTest,ShouldInsertAtIndex){
    int arrayForTest[]={1,2,3,4,5};
    LinkedListSequence<int> linkedListSequence=TestUtils::buildLinkedListSequence(arrayForTest);

    linkedListSequence.insertAtIndex(2,10);

    int arrayForValidation[]={1,2,10,3,4,5};
    for (size_t i = 0; i < 6; i++)
    {
        ASSERT_EQ(linkedListSequence.getAtIndex(i),arrayForValidation[i]);
    }
}

TEST(LinkedListSequence_DeleteAtIndexTest,ShouldDeleteAtCorrectIndex){
    int arrayForTest[]={1,2,3,4,5};
    LinkedListSequence<int> linkedListSequence=TestUtils::buildLinkedListSequence(arrayForTest);

    linkedListSequence.deleteAtIndex(2);

    int arrayForValidation[]={1,2,4,5};
    for (size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(linkedListSequence.getAtIndex(i),arrayForValidation[i]);
    }
}

TEST(LinkedListSequence_InsertLastTest,ShouldInsertAtLastIndex){
    int arrayForTest[]={1,2,3,4,5};
    LinkedListSequence<int> linkedListSequence=TestUtils::buildLinkedListSequence(arrayForTest);

    linkedListSequence.insertLast(10);

    int arrayForValidation[]={1,2,3,4,5,10};
    for (size_t i = 0; i < 6; i++)
    {
        ASSERT_EQ(linkedListSequence.getAtIndex(i),arrayForValidation[i]);
    }
}

TEST(LinkedListSequence_DeleteLastTest,ShouldDeleteLastIndex){
    int arrayForTest[]={1,2,3,4,5};
    LinkedListSequence<int> linkedListSequence=TestUtils::buildLinkedListSequence(arrayForTest);

    linkedListSequence.deleteLast();

    int arrayForValidation[]={1,2,3,4};
    for(size_t i = 0; i < 4; i++)
    {
        ASSERT_EQ(linkedListSequence.getAtIndex(i),arrayForValidation[i]);
    
    }
}