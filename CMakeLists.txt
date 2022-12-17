cmake_minimum_required(VERSION 3.21)
project(Librum VERSION 0.1 
               LANGUAGES CXX)



# Configuration
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
option(BUILD_TESTS ON)

string(CONCAT LIBRUM_WARNINGS "-Wall;-Wextra;-Wshadow;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;"
                              "-Wunused;-Woverloaded-virtual;-Wpedantic;"
                              "-Wnull-dereference;-Wdouble-promotion;-Wformat=2;-Wduplicated-cond;"
                              "-Wduplicated-branches;-Wlogical-op;-Wcast-qual;-Winit-self;"
                              "-Wunsafe-loop-optimizations;-Wmissing-include-dirs;-Wredundant-decls;"
                              "-Wswitch-enum;-Wunreachable-code;")


# Dependencies
add_subdirectory(libs/rapidfuzz-cpp)


# Build
add_subdirectory(src/)



# Tests
if(BUILD_TESTS)
    add_subdirectory(libs/googletest)
    add_subdirectory(tests/)
endif()
