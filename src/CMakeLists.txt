# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Configuration
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules" CACHE STRING "path to qml modules")



# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Quick Widgets Core5Compat REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick Widgets Core5Compat REQUIRED)



# Add subdirectories
add_subdirectory(application/)
add_subdirectory(model/)



# Building
qt_add_executable(appLibrum
                    presentation/qmlSources.qrc
                    ${PROJECT_SOURCE_DIR}/resources.qrc
                    main.cpp
)

target_compile_definitions(appLibrum
                PRIVATE
                        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_include_directories(appLibrum
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}/controller/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/application/include
)
target_link_libraries(appLibrum
                PRIVATE
                    application
                    Qt6::Quick
                    Qt6::Widgets
                    Qt6::Core5Compat
)

target_compile_options(appLibrum PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(appLibrum PRIVATE "$<$<CONFIG:RELEASE>:-O3>")




# Move fonts into binary dir
include(CopyRecursive)
set(FONTS_OUT_DIR ${PROJECT_BINARY_DIR}/resources/fonts)
file(MAKE_DIRECTORY ${FONTS_OUT_DIR})
message(DEBUG "Folder created at ${FONTS_OUT_DIR}")
copy_recursive(${PROJECT_SOURCE_DIR}/resources/fonts ${FONTS_OUT_DIR} "*.otf")