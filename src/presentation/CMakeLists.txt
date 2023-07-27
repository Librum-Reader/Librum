#Dependencies
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


# Build
add_library(presentation
                SHARED
                    ${PROJECT_SOURCE_DIR}/resources.qrc
                    qmlSources.qrc
                    modules/CppElements/key_sequence_recorder.cpp
                    modules/CppElements/key_sequence_recorder.hpp
                    modules/CppElements/sidebar_state.cpp
                    modules/CppElements/sidebar_state.hpp
                    modules/CppElements/document_item.cpp
                    modules/CppElements/document_item.hpp
                    modules/CppElements/page_item.cpp
                    modules/CppElements/page_item.hpp
)

target_compile_definitions(presentation PRIVATE PRESENTATION_LIBRARY)

target_compile_definitions(presentation
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(presentation
                PRIVATE
                    adapters
                    application
                    Qt6::Core
                    Qt6::Quick
)

target_include_directories(presentation
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/modules/CppElements
                    ${APPLICATION_INCLUDES}
)

target_compile_options(presentation PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_SANITIZERS};-Wall;-O0;-g>")
target_compile_options(presentation PRIVATE "$<$<CONFIG:RELEASE>:-O3>")
