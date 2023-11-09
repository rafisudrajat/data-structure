cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Lecture_Notes VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Main Main.cpp)
add_executable(Exercise1 ${CMAKE_SOURCE_DIR}/Exercise/Exercise1.cpp)

# Specify the include directory outside Lecture_2 folder
target_include_directories(Main PUBLIC ${CMAKE_SOURCE_DIR}/../Lecture_1)
target_include_directories(Exercise1 PUBLIC ${CMAKE_SOURCE_DIR})
add_library(SequenceLibrary STATIC
    ${CMAKE_SOURCE_DIR}/../Lecture_1/StaticArray.cpp
    ${CMAKE_SOURCE_DIR}/LinkedListSequence.h
)

target_link_libraries(Main PRIVATE SequenceLibrary)