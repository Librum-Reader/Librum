# Configuration
set(QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/presentation/modules" CACHE STRING "path to qml modules")


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Quick REQUIRED)



# Building
qt_add_executable(appLibrum
                    qmlSources.qrc
                    ${PROJECT_SOURCE_DIR}/resources.qrc
                    main.cpp
)

target_compile_definitions(appLibrum
                PRIVATE
                        $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_include_directories(appLibrum
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}/../controller/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../application/include
)
target_link_libraries(appLibrum
                PRIVATE
                    controller
                    application
                    Qt6::Quick
)
target_compile_options(appLibrum PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(appLibrum PRIVATE "$<$<CONFIG:RELEASE>:-O3>")