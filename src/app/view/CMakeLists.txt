cmake_minimum_required(VERSION 3.22)
project(Librum VERSION 0.1 LANGUAGES CXX)

# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/app/view/modules/" CACHE STRING "")



# Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# build options
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -flto")



# build
add_library(view SHARED
                        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_extensions/sidebarState.cpp
                        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_extensions/sidebarState.hpp
    )

target_compile_definitions(view
                    PRIVATE
                        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    )

target_link_libraries(view
                    PUBLIC 
                        Qt6::Quick
    )

target_include_directories(view
                    PUBLIC
                        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_extensions/

    )
