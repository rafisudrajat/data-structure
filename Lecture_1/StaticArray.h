#pragma once
#include<string>

namespace AlgorithmPractice {

    template<typename T>
    class StaticArray {
    private:
        // a pointer to an array
        T* elements;
        int numberOfElements;

    public:
        StaticArray(int numberOfElements) {
            elements = new T[numberOfElements];
            this->numberOfElements=numberOfElements;
        }

        ~StaticArray() {
            delete[] elements;
        }

        T getAt(int index) {
            if(index>=numberOfElements){
                throw "Index must be less than the number of elements";
            }

            return elements[index];
        }

        void setAt(int index, T value) {
            if(index>=numberOfElements){
                throw "Index must be less than the number of elements";
            }
  
            elements[index] = value;
        }
    };
}
