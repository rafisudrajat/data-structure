enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/include)
include(GoogleTest)

add_executable(
    InsertionSort_Test
    InsertionSort_Test.cpp
)
target_link_libraries(
    InsertionSort_Test
    GTest::gtest_main
    InsertionSort
)

gtest_discover_tests(InsertionSort_Test)

add_executable(
    SelectionSort_Test
    SelectionSort_Test.cpp
)
    
target_link_libraries(
    SelectionSort_Test
    GTest::gtest_main
    SelectionSort
)

gtest_discover_tests(SelectionSort_Test)