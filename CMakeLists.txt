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
add_executable(
    test_led_tests
    tests/test_power_led.cpp
)

add_executable(
    power_led
    examples/power_led.cpp
)

# Include header folder
target_include_directories(
    test_led_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(
    power_led
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link GoogleTest
target_link_libraries(
    test_led_tests
    gtest_main
)

# Auto-discover tests
include(GoogleTest)
gtest_discover_tests(test_led_tests)

add_test(NAME test_led_tests COMMAND test_led_tests)