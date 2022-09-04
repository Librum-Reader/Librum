# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)



# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Build
add_library(presentation
                SHARED
                    ${PROJECT_SOURCE_DIR}/resources.qrc
                    qmlSources.qrc
                    modules/CppElements/key_sequence_recorder.cpp
                    modules/CppElements/key_sequence_recorder.hpp
                    modules/CppElements/sidebar_state.cpp
                    modules/CppElements/sidebar_state.hpp
)

target_compile_definitions(presentation
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(presentation
                PRIVATE
                    adapters
                    Qt5::Core
                    Qt5::Quick
)

target_include_directories(presentation
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/modules/CppElements
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(presentation PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(presentation PRIVATE "$<$<CONFIG:RELEASE>:-O3>")