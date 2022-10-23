# Dependencies
find_package(QT NAMES Qt5 COMPONENTS Core Network Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network Gui REQUIRED)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Build
add_library(infrastructure
                SHARED
                    persistance/user_storage_access.cpp
                    persistance/user_storage_access.hpp
                    persistance/book_storage_access.cpp
                    persistance/book_storage_access.hpp
                    persistance/authentication_access.cpp
                    persistance/authentication_access.hpp
)

target_include_directories(infrastructure
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/persistance
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${ADAPTERS_INCLUDES}
)
target_compile_definitions(infrastructure
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(infrastructure
                PRIVATE
                    adapters
                    Qt5::Core
                    Qt5::Gui
                    Qt5::Network
)
target_compile_options(infrastructure PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(infrastructure PRIVATE "$<$<CONFIG:RELEASE>:-O3>")