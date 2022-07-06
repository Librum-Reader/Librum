cmake_minimum_required(VERSION 3.21)
project(Librum VERSION 0.1 
               LANGUAGES CXX)



# Language
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Configuration
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
option(BUILD_TESTS ON)



# Build
add_subdirectory(src/)



# Tests
if(BUILD_TESTS)
    add_subdirectory(tests/)
endif()