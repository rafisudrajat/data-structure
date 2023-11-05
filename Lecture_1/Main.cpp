#include "StaticArray.h"
#include "StructUtil.h"
#include <string>
#include <iostream>
using namespace AlgorithmPractice;

int main(){
    const int numberOfStudent=5;
    StaticArray<Util::Student> students(numberOfStudent) ;
    try{
        Util::Student testStudent = students.getAt(5);
    }catch(char* message){
        std::cout<<message<<"\n";
    }

    for(int i=0;i<numberOfStudent-2;i++){
        students.setAt(i,{"2002-0"+std::to_string(i)+"-09","Pajul"});
    }
    students.setAt(numberOfStudent-1,{"2002-01-09","Ipul"});
    StaticArray<Util::Student> recordedStudents(numberOfStudent);
    for(int i=0;i<numberOfStudent;i++){
        Util::Student currentStudent= students.getAt(i);
        for(int j=0;j<i;j++){
            if(currentStudent.birthDate==recordedStudents.getAt(j).birthDate){
                std::cout<< currentStudent.name <<"'s birthday is same with "<<students.getAt(j).name<<"\n";
                return 0;
            }
        }
        recordedStudents.setAt(i,currentStudent);
    }

    std::cout<<"There is no matching student"<<"\n";
}