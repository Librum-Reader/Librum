# Build
add_library(domain
                SHARED
                    user.cpp
                    user.hpp
)

target_include_directories(domain
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
)