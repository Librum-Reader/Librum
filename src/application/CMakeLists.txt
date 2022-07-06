# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Build
add_library(application
                SHARED
                    include/app_information.hpp
                    test_request.cpp
                    sidebar_state.cpp
                    include/sidebar_state.hpp
                    include/test_request.hpp
)

target_include_directories(application
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}/
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_definitions(application
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(application
                PRIVATE
                    Qt6::Core
                    Qt6::Network
)
target_compile_options(application PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(application PRIVATE "$<$<CONFIG:RELEASE>:-O3>")
