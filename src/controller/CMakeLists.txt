cmake_minimum_required(VERSION 3.21)

# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# build
add_library(controller 
                    SHARED
                        test_request.cpp
                        test_request.hpp
                        helper/sidebar_state.cpp
                        helper/sidebar_state.hpp
)

target_include_directories(controller
                    PUBLIC
                        ${CMAKE_CURRENT_SOURCE_DIR}
                        ${CMAKE_CURRENT_SOURCE_DIR}/helper
)

target_compile_definitions(controller
                    PRIVATE
                        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(controller
                    PUBLIC 
                        Qt6::Quick
                        infrastructure
)
target_compile_options(controller PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(controller PRIVATE "$<$<CONFIG:RELEASE>:-O3>")