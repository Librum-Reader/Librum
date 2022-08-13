# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_subdirectory(rum_content/)



# Build
add_library(domain
                SHARED
                    models/login_model.cpp
                    models/login_model.hpp
                    models/register_model.cpp
                    models/register_model.hpp
)

target_include_directories(domain
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/models
                    ${CMAKE_CURRENT_SOURCE_DIR}/rum_content
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(domain
                PRIVATE
                    rum_content
                    Qt6::Core
)