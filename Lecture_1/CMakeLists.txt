cmake_minimum_required(VERSION 3.15)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Main_Lecture1 Main.cpp)
target_include_directories(Main_Lecture1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Utils)
add_library(StaticArray STATIC StaticArray.cpp)

target_link_libraries(Main_Lecture1 PRIVATE StaticArray)