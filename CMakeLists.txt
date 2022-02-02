cmake_minimum_required(VERSION 3.22)
project(Librum VERSION 0.1 LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH "/home/creapermann/Qt/6.2.3/gcc_64")  # remove


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



# option
option(BUILD_TESTS ON)



# Dependencies
include(FetchContent)
find_package(QT NAMES Qt6 COMPONENTS Core Quick WebEngine LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick LinguistTools REQUIRED)
FetchContent_Declare(googletest
                    GIT_REPOSITORY https://github.com/google/googletest.git
                    GIT_TAG        f2d71f0be2ff2230789cfaf37d4c4f38db5b1c7c
    )

FetchContent_MakeAvailable(googletest)



# build
qt_add_executable(appLibrum
    main.cpp
)

qt_add_qml_module(appLibrum
    URI Librum
    VERSION 1.0
    QML_FILES main.qml 
)

target_compile_definitions(appLibrum
                    PRIVATE 
                        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    )
target_link_libraries(appLibrum
                    PRIVATE 
                        Qt6::Quick
    )



# tests
if(BUILD_TESTS)

endif()
