cmake_minimum_required(VERSION 3.15)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Main_Lecture2 Main.cpp)
add_executable(Exercise1 ${CMAKE_CURRENT_SOURCE_DIR}/Exercise/Exercise1.cpp)
add_executable(Exercise2 
    ${CMAKE_CURRENT_SOURCE_DIR}/Exercise/Exercise2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Exercise/SetFromSequence.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Exercise/Pair.h
)

# Specify the include directory outside Lecture_2 folder
target_include_directories(Main_Lecture2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Lecture_1)
target_include_directories(Exercise1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(SequenceLibrary STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../Lecture_1/StaticArray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LinkedListSequence.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AmmortizedDynamicArray.h

)

target_link_libraries(Main_Lecture2 PRIVATE SequenceLibrary)