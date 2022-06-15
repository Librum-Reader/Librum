cmake_minimum_required(VERSION 3.21)
project(Librum VERSION 0.1 LANGUAGES CXX)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/presentation/modules" CACHE STRING "path to qml modules")
list(APPEND CMAKE_PREFIX_PATH "/home/creapermann/Qt/6.2.3/gcc_64")


# Language
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Configuration
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
option(BUILD_TESTS ON)



# Build
add_subdirectory(src/)



# Tests
if(BUILD_TESTS)
    add_subdirectory(tests/)
endif()
