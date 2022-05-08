cmake_minimum_required(VERSION 3.21)


# Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Build
add_library(domain
                SHARED
)

target_include_directories(domain
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_compile_options(domain PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(domain PRIVATE "$<$<CONFIG:RELEASE>:-O3>")