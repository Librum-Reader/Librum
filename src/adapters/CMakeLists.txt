# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)


# Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)



# Build
add_library(adapters
                SHARED
                    controllers/authentication_controller.cpp
                    controllers/authentication_controller.hpp
)

target_include_directories(adapters
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(adapters
                PRIVATE
                    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)
target_link_libraries(adapters
                PRIVATE
                    Qt6::Core
)
target_compile_options(adapters PRIVATE "$<$<CONFIG:DEBUG>:-Wall;-Wextra;-g3;-Og>")
target_compile_options(adapters PRIVATE "$<$<CONFIG:RELEASE>:-O3>")