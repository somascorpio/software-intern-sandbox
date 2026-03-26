cmake_minimum_required(VERSION 3.10)
project(software-intern-sandbox)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add test executable
add_executable(runTests
    tests/test_power_led.cpp
    examples/power_led.cpp
)

# Include header folder
target_include_directories(runTests PRIVATE include)

# Link GoogleTest
target_link_libraries(runTests gtest gtest_main)

#Register Test
add_test(NAME runTests COMMAND runTests)

# Auto-discover tests
include(GoogleTest)
gtest_discover_tests(runTests)