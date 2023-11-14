cmake_minimum_required(VERSION 3.14)
# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set the project name and version
project(Lecture_Notes VERSION 1.0)
# Set the policy to NEW
cmake_policy(SET CMP0135 NEW)

add_subdirectory(Lecture_1)
add_subdirectory(Lecture_2)
add_subdirectory(Lecture_3)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b10fad38c4026a29ea6561ab15fc4818170d1c10.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)