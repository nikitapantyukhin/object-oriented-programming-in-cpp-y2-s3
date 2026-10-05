cmake_minimum_required(VERSION 3.16)

project(lab_01)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lab01_lib
    src/str_ops.cpp
)

target_include_directories(lab01_lib PUBLIC src)

add_executable(lab01_main
    main.cpp
)

target_link_libraries(lab01_main
    lab01_lib
)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(lab01_tests
    tests/test_variant.cpp
)

target_link_libraries(lab01_tests
    lab01_lib
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(lab01_tests)