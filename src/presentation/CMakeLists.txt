#Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)



############ Building TreeView ############
#
# Since the QtQuick.TreeView is only an add-on to Qt5, and not a real QtQuick type,
# we need to build it our selves.
# Sadly QtQuick.TreeView can only be built with qmake in Qt5, so we need to invoke
# qmake via "add_custom_target" from cmake and build it that way.
#
#
# TreeView variables
set(TREEVIEW_DEST ${CMAKE_CURRENT_BINARY_DIR}/qt_tree_view)
set(TREEVIEW_BIN ${CMAKE_CURRENT_BINARY_DIR}/qt_tree_view/qml/QtQuick/TreeView/)
set(TREEVIEW_SRC ${CMAKE_CURRENT_SOURCE_DIR}/modules/CppElements/qttreeview)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} ${TREEVIEW_BIN}/qt_tree_view/qml/QtQuick/TreeView)
#
#
# Build TreeView by invoking qmake via add_custom_target
add_custom_target(qt_treeview_qmake ALL
    COMMAND mkdir -p ${TREEVIEW_DEST}                   # Create the destination folder
    COMMAND qmake -o ${TREEVIEW_DEST} ${TREEVIEW_SRC}   # Run qmake to generate the build files
    COMMAND make -C ${TREEVIEW_DEST}                    # Build the TreeView
    VERBATIM
    USES_TERMINAL
)
#
# Copy the necessary qt5/ folder with TreeView.qml, which is needed by the qmldir to its destination
file(COPY ${TREEVIEW_SRC}/src/qt5 DESTINATION ${TREEVIEW_BIN})
#
#
# Create a qmldir placeholder file at the final location, because "add_library" needs it
# at configuration time, but "add_custom_target" only generates it at build time.
# This file is only needed so that "add_library" doesn't fail, it is replaced at build time.
# To not override the "real" file everytime you reconfigure, check if the file already exists.
if(NOT EXISTS "${TREEVIEW_BIN}/qmldir")
    file(WRITE ${TREEVIEW_BIN}/qmldir "")
endif()
#
###########################################




# Build
add_library(presentation
                SHARED
                    ${PROJECT_SOURCE_DIR}/resources.qrc
                    ${TREEVIEW_BIN}/qmldir
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

add_dependencies(presentation qt_treeview_qmake)


target_compile_definitions(presentation
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(presentation
                PRIVATE
                    adapters
                    application
                    Qt5::Core
                    Qt5::Quick
)

target_include_directories(presentation
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/modules/CppElements
                    ${APPLICATION_INCLUDES}
                    ${OKULARCORE_INCLUDES}
                    
                    # Need Qt's private API headers for qquicktableviewextra
                    ${Qt${QT_VERSION_MAJOR}Quick_PRIVATE_INCLUDE_DIRS}
)

target_compile_options(presentation PRIVATE "$<$<CONFIG:DEBUG>:${LIBRUM_SANITIZERS};-Wall;-Wextra;-O0;-g>")
target_compile_options(presentation PRIVATE "$<$<CONFIG:RELEASE>:-O3>")