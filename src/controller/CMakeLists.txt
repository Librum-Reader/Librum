# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Build
add_library(controller
                SHARED
                    test_request.cpp
                    helper/sidebar_state.cpp
                    include/sidebar_state.hpp
                    include/test_request.hpp
)
target_include_directories(controller
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../application/include
)
target_compile_definitions(controller
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(controller
                PRIVATE
                    application
                    Qt6::Core
                    Qt6::Network
)
target_compile_options(controller PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(controller PRIVATE "$<$<CONFIG:RELEASE>:-O3>")