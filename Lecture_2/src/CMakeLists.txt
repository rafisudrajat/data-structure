# add the executable
add_executable(Main_Lecture2 Main.cpp)
target_include_directories(Main_Lecture2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(Exercise1 ${CMAKE_CURRENT_SOURCE_DIR}/Exercise/Exercise1.cpp)
add_executable(Exercise2 ${CMAKE_CURRENT_SOURCE_DIR}/Exercise/Exercise2.cpp)
target_include_directories(Exercise2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Specify the include directory outside Lecture_2 folder
target_include_directories(Main_Lecture2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../Lecture_1)
target_include_directories(Exercise1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(SequenceLibrary STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Lecture_1/StaticArray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/LinkedListSequence.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/AmmortizedDynamicArray.h

)

target_link_libraries(Main_Lecture2 PRIVATE SequenceLibrary)