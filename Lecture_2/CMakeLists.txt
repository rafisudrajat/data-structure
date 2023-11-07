cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Lecture_Notes VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Main Main.cpp)
# Specify the include directory outside Lecture_2 folder
target_include_directories(Main PUBLIC ${CMAKE_SOURCE_DIR}/../Lecture_1)
add_library(SequenceLibrary STATIC
    ${CMAKE_SOURCE_DIR}/../Lecture_1/StaticArray.cpp
    ${CMAKE_SOURCE_DIR}/LinkedListSequence.h
)

target_link_libraries(Main PRIVATE SequenceLibrary)