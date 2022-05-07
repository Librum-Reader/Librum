cmake_minimum_required(VERSION 3.21)
project(Librum VERSION 0.1 LANGUAGES CXX)

# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# build
add_library(presentation SHARED
                        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_extensions/sidebar_state.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_extensions/sidebar_state.hpp
)

target_compile_definitions(presentation
                    PRIVATE
                        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(presentation
                    PUBLIC 
                        Qt6::Quick
)

target_include_directories(presentation
                    PUBLIC
                        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_extensions/

)
target_compile_options(presentation PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(presentation PRIVATE "$<$<CONFIG:RELEASE>:-O3>")