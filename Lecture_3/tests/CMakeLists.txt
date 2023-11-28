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
        
add_executable(
    PermutationSort_Test
    PermutationSort_Test.cpp
)

target_link_libraries(
    PermutationSort_Test
    GTest::gtest_main
    PermutationSort
)

gtest_discover_tests(PermutationSort_Test)

add_executable(
    MergeSort_Test
    MergeSort_Test.cpp
)

target_link_libraries(
    MergeSort_Test
    GTest::gtest_main
)

gtest_discover_tests(MergeSort_Test)

add_executable(
    Utils_Test
    Utils_Test.cpp
)

target_link_libraries(
    Utils_Test
    GTest::gtest_main
    Utils
)

gtest_discover_tests(Utils_Test)