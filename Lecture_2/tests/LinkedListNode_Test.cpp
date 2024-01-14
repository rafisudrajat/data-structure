#include <gtest/gtest.h>
#include "LinkedListNode.h"
using namespace AlgorithmPractice;

TEST(LinkedListNode, ShouldBuildNodeWithValue) {
    LinkedListNode<int> node(5,nullptr);

    ASSERT_EQ(node.value,5);
}

TEST(LinkedListNode, ShouldHaveValidNextNode){
    LinkedListNode<int>* nextNode= new LinkedListNode<int>(10);
    
    LinkedListNode<int> node(5,nextNode);

    ASSERT_NE(node.nextNode,nullptr);
    ASSERT_EQ(node.nextNode->value,10);
}
