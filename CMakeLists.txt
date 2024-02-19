cmake_minimum_required(VERSION 3.21)
project(Librum VERSION 0.1 
               LANGUAGES CXX)


# Set output directory
if(NOT ANDROID AND NOT IOS)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}")
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}")
    SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}")
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}")
endif()


# Configuration
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
option(BUILD_TESTS ON)
option(NO_VENV OFF)
option(ENABLE_COVERAGE OFF)
option(USE_SANITIZERS OFF) # Slows down program startup significantly


# Qt
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Widgets Network Gui LinguistTools QuickControls2 Test)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Create a set of warnings for windows systems
if(WIN32)
    add_compile_options(-Zc:rvalueCast -Zc:inline -Zc:strictStrings -Zc:throwingNew -permissive- -Zc:__cplusplus -Zc:externConstexpr -utf-8 -w34100 -w34189 -w44996 -w44456 -w44457 -w44458)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Create a set of warnings for unix systems
if(UNIX)
    string(CONCAT LIBRUM_WARNINGS "-Wall;-Wextra;-Wshadow;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;"
                                  "-Wunused;-Woverloaded-virtual;-Wpedantic;-Wmissing-include-dirs;"
                                  "-Wnull-dereference;-Wformat=2;-Wcast-qual;-Winit-self;"
                                  "-Wswitch-enum;-Wunreachable-code;-Wredundant-decls;")

    # GCC specific warnings
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # If compiler is GCC
        list(APPEND LIBRUM_WARNINGS "-Wduplicated-cond")
        list(APPEND LIBRUM_WARNINGS "-Wduplicated-branches")
        list(APPEND LIBRUM_WARNINGS "-Wunsafe-loop-optimizations")
        list(APPEND LIBRUM_WARNINGS "-Wlogical-op")
        list(APPEND LIBRUM_WARNINGS "-Wunsafe-loop-optimizations")
    endif()
endif()


# Create variables for coverage compiler and linker flags
if(ENABLE_COVERAGE)
    string(CONCAT COVERAGE_COMPILE_OPTIONS "-fprofile-arcs;-ftest-coverage;-fno-inline;--coverage")
    string(CONCAT COVERAGE_LINKING_OPTIONS "-lgcov;--coverage;")
endif()


# Create a set of sanitizers
if(USE_SANITIZERS)
    string(CONCAT LIBRUM_SANITIZERS "-fsanitize=address,undefined,shift,integer-divide-by-zero,"
                                    "float-divide-by-zero,unreachable,vla-bound,null,return,leak,"
                                    "bounds,float-cast-overflow,enum")
endif()


# Dependencies
add_subdirectory(libs/rapidfuzz-cpp)


# Build
add_subdirectory(src/)

# Tests
if(BUILD_TESTS)
    include(CTest)
    add_subdirectory(libs/googletest)
    add_subdirectory(tests/)
endif()


if(WIN32)
    configure_file("${PROJECT_SOURCE_DIR}/unzip_and_move.bat" "${PROJECT_BINARY_DIR}/unzip_and_move.bat" COPYONLY)
endif()
