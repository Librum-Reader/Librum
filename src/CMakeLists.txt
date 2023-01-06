# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Configuration
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/presentation/modules;${CMAKE_CURRENT_BINARY_DIR}/" CACHE STRING "Qml modules")


# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Quick Widgets QuickControls2 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick Widgets QuickControls2 REQUIRED)



# Language
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



# Configure and add subdirectories
add_subdirectory(domain/)
get_target_property(DOMAIN_INCLUDES domain INTERFACE_INCLUDE_DIRECTORIES)

add_subdirectory(application/)
get_target_property(APPLICATION_INCLUDES application INTERFACE_INCLUDE_DIRECTORIES)

add_subdirectory(infrastructure/)
get_target_property(INFRASTRUCTURE_INCLUDES infrastructure INTERFACE_INCLUDE_DIRECTORIES)

add_subdirectory(adapters/)
get_target_property(ADAPTERS_INCLUDES adapters INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(ADAPTERS_BIN_DIR adapters CMAKE_BINARY_DIR)

add_subdirectory(presentation/)




# Building
set(OKULARCORE_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/application/core
    ${CMAKE_CURRENT_SOURCE_DIR}/application/core/core
    ${CMAKE_CURRENT_SOURCE_DIR}/application/core/gui
    ${CMAKE_CURRENT_BINARY_DIR}/application/core
    ${CMAKE_CURRENT_BINARY_DIR}/application/core/core
)

add_executable(appLibrum
    main.cpp
    dependency_injection.hpp
    message_handler.hpp
)

target_compile_definitions(appLibrum
    PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(appLibrum
    PRIVATE
        presentation
        adapters
        application
        infrastructure
        okularpart
        Qt5::Quick
        Qt5::Widgets
        Qt5::QuickControls2
)

target_include_directories(appLibrum
    PRIVATE
        ${ADAPTERS_INCLUDES}
        ${APPLICATION_INCLUDES}
        ${INFRASTRUCTURE_INCLUDES}
        ${OKULARCORE_INCLUDES}
)

target_compile_options(appLibrum PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(appLibrum PRIVATE "$<$<CONFIG:RELEASE>:-O3>")




# Move fonts into binary dir    
include(CopyRecursive)
set(FONTS_OUT_DIR ${PROJECT_BINARY_DIR}/resources/fonts)
file(MAKE_DIRECTORY ${FONTS_OUT_DIR})
message(DEBUG "Folder created at ${FONTS_OUT_DIR}")
copy_recursive(${PROJECT_SOURCE_DIR}/resources/fonts ${FONTS_OUT_DIR} "*.otf")