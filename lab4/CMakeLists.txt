
cmake_minimum_required(VERSION 3.20)

project(lab_04 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(lab04_lib
    src/bank_product.cpp
    src/deposit.cpp
    src/credit.cpp
    src/card.cpp
    src/bank_collection.cpp
)

target_include_directories(lab04_lib PUBLIC src)

add_executable(lab04_main main.cpp)
target_link_libraries(lab04_main lab04_lib)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(lab04_tests
    tests/test_variant.cpp
)

target_link_libraries(lab04_tests
    lab04_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lab04_tests)
