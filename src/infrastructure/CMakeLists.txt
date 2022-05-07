cmake_minimum_required(VERSION 3.21)

# Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Build
add_library(infrastructure
                SHARED
)

target_include_directories(infrastructure
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/
)
target_link_directories(infrastructure
                PRIVATE
                    application
)

target_compile_options(infrastructure PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(infrastructure PRIVATE "$<$<CONFIG:RELEASE>:-O3>")