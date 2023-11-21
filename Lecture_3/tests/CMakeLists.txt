enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/include)

add_executable(
    InsertionSort_Test
    InsertionSort_Test.cpp
)
target_link_libraries(
    InsertionSort_Test
    GTest::gtest_main
    InsertionSort
)
include(GoogleTest)
gtest_discover_tests(InsertionSort_Test)