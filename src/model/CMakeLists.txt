cmake_minimum_required(VERSION 3.22)

# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# build options
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -flto")



# build
add_library(model SHARED
                        model.cpp
                        model.hpp
    )

target_include_directories(model
                    PUBLIC
                        ${CMAKE_CURRENT_SOURCE_DIR}/
    )