cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(Lecture_Notes VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(Main Main.cpp)
target_include_directories(Main PUBLIC "${PROJECT_SOURCE_DIR}/Utils")
add_library(StaticArray STATIC StaticArray.cpp)

target_link_libraries(Main PRIVATE StaticArray)