#Dependencies
find_package(ECM ${KF5_REQUIRED_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

find_package(QT NAMES Qt5 COMPONENTS Core Quick Svg Xml REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick Svg Xml REQUIRED)
find_package(KF5Bookmarks)
find_package(KF5 REQUIRED COMPONENTS
    Bookmarks
    Service
    I18n
)


# Options
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/qt_tree_view/qml/QtQuick/TreeView)
set(TREEVIEW_DEST ${CMAKE_CURRENT_BINARY_DIR}/qt_tree_view)
set(TREEVIEW_SRC ${CMAKE_CURRENT_SOURCE_DIR}/modules/CppElements/qttreeview)


# Build
add_library(presentation
                SHARED
                    ${PROJECT_SOURCE_DIR}/resources.qrc
                    qmlSources.qrc
                    modules/CppElements/key_sequence_recorder.cpp
                    modules/CppElements/key_sequence_recorder.hpp
                    modules/CppElements/sidebar_state.cpp
                    modules/CppElements/sidebar_state.hpp
                    modules/CppElements/page_item.cpp
                    modules/CppElements/page_item.hpp
                    modules/CppElements/document_item.cpp
                    modules/CppElements/document_item.hpp
)

target_compile_definitions(presentation
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)


# Add treeview
add_custom_target(qt_treeview_qmake ALL
    COMMAND mkdir -p ${TREEVIEW_DEST}
    COMMAND qmake -o ${TREEVIEW_DEST} ${TREEVIEW_SRC}
    COMMAND make -C ${TREEVIEW_DEST}
    COMMAND make install -i -C ${TREEVIEW_DEST}
    VERBATIM
    USES_TERMINAL
)



# Make presentation depend on qt_treeview_qmake
add_dependencies(presentation qt_treeview_qmake)



# Link
target_link_libraries(presentation
                PRIVATE
                    adapters
                    okularcore
                    okularpart
                    Qt5::Core
                    Qt5::Quick
                    Qt5::Qml
                    Qt5::Xml
                    Qt5::Svg
                    KF5::Bookmarks
                    KF5::I18n
                    KF5::Service
)

target_include_directories(presentation
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/modules/CppElements
                    ${APPLICATION_INCLUDES}
                    ${OKULARCORE_INCLUDES}
)

target_compile_options(presentation PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_SANITIZERS};-Wall;-Wextra;-g3;-Og>")
target_compile_options(presentation PRIVATE "$<$<CONFIG:RELEASE>:-O3>")