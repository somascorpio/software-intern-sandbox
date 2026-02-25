cmake_minimum_required(VERSION 3.10.0)
project(intern-sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(hello_world hello_world.cpp)
add_executable(power_led power_led.cpp)
add_executable(power_led_test tests/power_led_test.cc)

target_link_libraries(
  power_led_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(power_led_test)

