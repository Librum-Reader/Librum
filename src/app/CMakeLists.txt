cmake_minimum_required(VERSION 3.22)

add_subdirectory(model)
add_subdirectory(view)
add_subdirectory(controller)

set(QML_SOURCES ${QML_SOURCES} PARENT_SCOPE)   # Bringing variable to the next level