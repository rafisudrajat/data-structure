namespace AlgorithmPractice{

template<typename ValueType>
class LinkedListNode
{
public:
    ValueType value;
    LinkedListNode<ValueType>* nextNode;
    LinkedListNode(ValueType value, LinkedListNode<ValueType>* nextNode){
        this->value=value;
        this->nextNode=nextNode;
    }
    ~LinkedListNode(){
        delete nextNode;
    }
};
}
