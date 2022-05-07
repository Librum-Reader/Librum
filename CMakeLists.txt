cmake_minimum_required(VERSION 3.21)
project(Librum VERSION 0.1 LANGUAGES CXX)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/presentation/modules" CACHE STRING "path to qml modules")


# Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Build options
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)



# Options
option(BUILD_TESTS ON)



# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core Quick WebEngine REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)
add_subdirectory(src/controller)
add_subdirectory(src/domain)
add_subdirectory(src/application)
add_subdirectory(src/infrastructure)



# Building
qt_add_executable(appLibrum
    resources.qrc
    src/main.cpp
)

target_compile_definitions(appLibrum
                    PRIVATE
                        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(appLibrum
                    PRIVATE
                        Qt6::Quick
                        controller      # For registration to QML engine
)
target_compile_options(appLibrum PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(appLibrum PRIVATE "$<$<CONFIG:RELEASE>:-O3>")



# Move fonts into binary dir
include(CopyRecursive)
set(FONTS_OUT_DIR ${CMAKE_BINARY_DIR}/resources/fonts)
file(MAKE_DIRECTORY ${FONTS_OUT_DIR})
message(DEBUG "Folder created at ${FONTS_OUT_DIR}")
copy_recursive(${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts ${FONTS_OUT_DIR} "*.otf")



# Tests
if(BUILD_TESTS)
    add_subdirectory(tests/)
endif()
