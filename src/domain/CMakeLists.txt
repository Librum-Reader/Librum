# Dependencies
find_package(QT NAMES Qt6 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)



# Build
add_library(domain
                SHARED
                    models/login_model.cpp
                    models/login_model.hpp
)

target_include_directories(domain
                PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/models
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(domain
                PRIVATE
                    Qt6::Core
)