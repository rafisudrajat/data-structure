enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include(GoogleTest)

add_executable(
    LinkedListNode_Test
    LinkedListNode_Test.cpp
)
target_link_libraries(
    LinkedListNode_Test
    GTest::gtest_main
)

add_executable(
    LinkedListSequence_Test
    LinkedListSequence_Test.cpp
)
target_link_libraries(
    LinkedListSequence_Test
    GTest::gtest_main
)

add_executable(
    ArraySequence_Test
    ArraySequence_Test.cpp
)
target_link_libraries(
    ArraySequence_Test
    GTest::gtest_main
)