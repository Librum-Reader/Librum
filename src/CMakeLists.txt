# Configuration
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/presentation/modules ${CMAKE_CURRENT_BINARY_DIR}/" CACHE STRING "Qml modules")


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

# Set WIN32 for windows to avoid console getting opened on start on windows
if(WIN32)
    set_target_properties(librum PROPERTIES WIN32_EXECUTABLE true)
endif()

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
if(UNIX)
    set(FONTS_OUT_DIR "$ENV{HOME}/.local/share/Librum-Reader/Librum/resources/fonts")
elseif(WIN32)
   set(FONTS_OUT_DIR "$ENV{HOMEPATH}/AppData/Roaming/Librum-Reader/Librum/resources/fonts")
endif()

include(CopyRecursive)
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

    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/logo.svg
            DESTINATION share/pixmaps
            RENAME librum.svg
            )
        
    install(FILES ${PROJECT_SOURCE_DIR}/librum.desktop
            DESTINATION share/applications
            )
endif()
