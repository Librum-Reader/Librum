cmake_minimum_required(VERSION 3.21)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


# Language
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Build
add_library(infrastructure
                SHARED
                    test_class.hpp
                    test_class.cpp
)

target_include_directories(infrastructure
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/
)
target_compile_definitions(infrastructure
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(infrastructure
                PUBLIC
                    Qt6::Quick
                    # application
)
target_compile_options(infrastructure PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(infrastructure PRIVATE "$<$<CONFIG:RELEASE>:-O3>")