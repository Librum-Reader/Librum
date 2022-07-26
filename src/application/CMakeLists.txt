# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core Quick Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Network REQUIRED)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Build
add_library(application
                SHARED
                    app_information.hpp
                    models/chapter_tree/chapter_tree_model.cpp
                    models/chapter_tree/chapter_tree_node.cpp
                    models/chapter_tree/chapter_tree_model.hpp
                    models/chapter_tree/chapter_tree_node.hpp
                    sidebar_state.hpp
                    test_request.hpp
                    test_request.cpp
                    sidebar_state.cpp
)

target_include_directories(application
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/models/chapter_tree
)
target_compile_definitions(application
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(application
                PRIVATE
                    Qt6::Core
                    Qt6::Network
                    Qt6::Quick
)
target_compile_options(application PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(application PRIVATE "$<$<CONFIG:RELEASE>:-O3>")



# Qml-Module
qt6_add_qml_module(application
  URI "Librum.Models"
  VERSION 1.0
)