cmake_minimum_required(VERSION 3.20)

project(lab_03 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lab03_lib
    src/temperature.cpp
    src/temperature_set.cpp
)

target_include_directories(lab03_lib PUBLIC src)

add_executable(lab03_main
    main.cpp
)

target_link_libraries(lab03_main
    lab03_lib
)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(lab03_tests
    tests/test_variant.cpp
)

target_link_libraries(lab03_tests
    lab03_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lab03_tests)
