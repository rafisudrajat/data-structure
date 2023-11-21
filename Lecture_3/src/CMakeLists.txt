# add the executable
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(Utils Utils.cpp)

add_library(PermutationSort PermutationSort.cpp Utils)

add_library(SelectionSort SelectionSort.cpp Utils)

add_executable(Main_Lecture3 Main.cpp Utils)

# Add the source files
add_library(InsertionSort InsertionSort.cpp Utils)