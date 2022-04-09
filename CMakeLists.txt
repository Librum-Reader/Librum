cmake_minimum_required(VERSION 3.22)
project(Librum VERSION 0.1 LANGUAGES CXX)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/app/view/modules" CACHE STRING "path to qml modules")


# Language
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Build options
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Og")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -flto")



# Options
option(BUILD_TESTS ON)



# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core Quick WebEngine LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools REQUIRED)
add_subdirectory(src/app/model)
add_subdirectory(src/app/view)
add_subdirectory(src/app/controller)



# Building
qt_add_executable(appLibrum
    src/app/main.cpp
    resources.qrc
)

target_compile_definitions(appLibrum
                    PRIVATE
                        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    )
target_link_libraries(appLibrum
                    PRIVATE
                        Qt6::Quick
                        controller
                        view
    )
target_include_directories(appLibrum
                    PRIVATE
                        src/app/view/extensions/
    )


# Tests
if(BUILD_TESTS)
    add_subdirectory(tests/)
endif()
