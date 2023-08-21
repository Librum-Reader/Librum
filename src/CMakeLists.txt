# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Configuration
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/presentation/modules ${CMAKE_CURRENT_BINARY_DIR}/" CACHE STRING "Qml modules")


# Dependencies
find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Widgets QuickControls2)



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
set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")
qt_add_executable(librum
    main.cpp
    dependency_injection.hpp
    message_handler.hpp
    ${app_icon_resource_windows}
)

target_compile_definitions(librum
    PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(librum
    PRIVATE
        presentation
        adapters
        application
        infrastructure
        ${MUPDF_OUTPUT}
        Qt6::Quick
        Qt6::Widgets
        Qt6::QuickControls2
)

target_include_directories(librum
    PRIVATE
        ${ADAPTERS_INCLUDES}
        ${APPLICATION_INCLUDES}
        ${INFRASTRUCTURE_INCLUDES}
)

target_compile_options(librum PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_SANITIZERS};-O0;-g>")
target_compile_options(librum PRIVATE "$<$<CONFIG:RELEASE>:-O3>")

target_link_options(librum PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_SANITIZERS}>")



# Move fonts into binary dir    
include(CopyRecursive)
set(FONTS_OUT_DIR ${PROJECT_BINARY_DIR}/resources/fonts)
file(MAKE_DIRECTORY ${FONTS_OUT_DIR})
message(DEBUG "Folder created at ${FONTS_OUT_DIR}")
copy_recursive(${PROJECT_SOURCE_DIR}/resources/fonts ${FONTS_OUT_DIR} "*.otf")


# Install rules
if(UNIX)
    install(TARGETS librum
            DESTINATION bin
            LIBRARY DESTINATION lib
            RUNTIME DESTINATION bin
            )
endif()