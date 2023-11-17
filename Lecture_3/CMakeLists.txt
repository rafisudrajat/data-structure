cmake_minimum_required(VERSION 3.15)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(PermutationSort PermutationSort.cpp)
add_executable(SelectionSort SelectionSort.cpp)
add_executable(InsertionSort InsertionSort.cpp)
add_executable(Main_Lecture3 Main.cpp)