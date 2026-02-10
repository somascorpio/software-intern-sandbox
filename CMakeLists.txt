cmake_minimum_required(VERSION 3.10.0)
project(intern-sandbox VERSION 0.1.0 LANGUAGES C CXX)

add_executable(hello_world hello_world.cpp)
add_executable(power_led power_led.cpp)

include(CTest)
enable_testing()

