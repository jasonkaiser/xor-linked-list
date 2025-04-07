cmake_minimum_required(VERSION 3.28)
project(Homework1)

set(CMAKE_CXX_STANDARD 20)

# Fetch doctest
include(FetchContent)
FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.11  # Use the latest stable version
)
FetchContent_MakeAvailable(doctest)

# Enable testing
enable_testing()

add_executable(Exe src/main.cpp)
add_executable(Test test/tests.cpp)
add_library(Lib INTERFACE
        include/Node.h
        include/XorLinkedList.h
        include/XorLinkedList.tpp)
target_link_libraries(Exe PRIVATE Lib)
target_link_libraries(Test PRIVATE Lib doctest::doctest)

add_test(NAME XorLinkedListTests COMMAND Test)