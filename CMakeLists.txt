cmake_minimum_required(VERSION 3.10)

project(locjup VERSION 0.1.0)

configure_file(locjup.h.in locjup.h)
add_executable(locjup locjup.cpp)
target_include_directories(locjup PUBLIC "${PROJECT_BINARY_DIR}")
set(CMAKE_CPP_STANDARD 11)
set(CMAKE_CPP_STANDARD_REQUIRED True)
install(TARGETS locjup DESTINATION bin)
